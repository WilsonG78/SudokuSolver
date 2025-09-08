#include "DLX.h"


//Stack of Node* implementation to take care of nodes used in solution
Stack* init_stack(){
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    stack->head = NULL;
    stack->tail = NULL;
    stack->size =0;
    return stack;
}


void push_stack(Stack * stack,Node *node){
    Vertex *new_vertex = (Vertex*)malloc(sizeof(Vertex));
    new_vertex->data = node;
    new_vertex->next = NULL;
    new_vertex->prev = NULL;
    if(!stack->head){
        stack->head = new_vertex;
        stack->tail = new_vertex;
    }
    else{
        stack->tail->next = new_vertex;
        new_vertex->prev = stack->tail;
        stack->tail = new_vertex;
    }
    stack->size++;
}

Node* pop_stack(Stack *stack){
    if(!stack || !stack->tail){
        return NULL;
    }
    
    Vertex *to_del = stack->tail;
    Node *to_return = to_del->data;
    if(stack->head == stack->tail){
        free(to_del);
        stack->head = NULL;
        stack->tail = NULL;
    }
    else{
        stack->tail = to_del->prev;
        stack->tail->next = NULL;
        free(to_del);
    }
    stack->size--;

    return to_return;
}

void from_stack_to_ListOfLists(Stack* stack , ListOfLists *list_of_lists){
    Vertex *curr = stack->head;
    List *new_list = init_List();
    while(curr){
        if(curr->data && curr->data){
            Node *node = curr->data;
            push_node(new_list,node);
        }
        curr = curr->next;
    }
    push_list(list_of_lists,new_list);
}



//public
DLX* init_DLX(Array2D* problem_matrix){
    if(!problem_matrix){
        perror("Problem Matrix is NULL");
        return NULL;
    }
    DLX *dlx = (DLX*)malloc(sizeof(DLX));


    int cols = problem_matrix->cols, rows = problem_matrix->rows;
    dlx->cols = cols;
    dlx->rows = rows;
    dlx->solution = (Node **)malloc(rows * sizeof(Node*));
    dlx->solutionSize = 0;
    
    dlx->root = (ColNode *)malloc(sizeof(ColNode));
    dlx->root->node = (Node*)malloc(sizeof(Node));
    dlx->root->node->colID = -1;
    dlx->root->node->rowID = -1;
    dlx->root->node->column = dlx->root;
    dlx->root->size = 0;
    dlx->root->node->left = dlx->root->node;
    dlx->root->node->right = dlx->root->node;
    dlx->root->node->up = dlx->root->node;
    dlx->root->node->down = dlx->root->node;

    //init columns
    dlx->columns =(ColNode *)malloc(sizeof(ColNode) * cols);
    //We define helper nodes to have better access to next add node in dancing links implementation
    Node **helper_nodes_cols = (Node**)malloc(sizeof(Node*) *cols );
    Node **helper_nodes_rows = (Node**)malloc(sizeof(Node*) *rows );
    Node **first_in_row = (Node**)malloc(sizeof(Node*) *rows );


    for(int j=0 ; j < cols;j++){
        dlx->columns[j].node = (Node * )malloc(sizeof(Node));
        ColNode *curr_col = &dlx->columns[j];
        curr_col->node->down =curr_col->node;
        curr_col->node->up = curr_col->node;
        curr_col->size = 0;
        curr_col->node->right = (j == cols-1) ? dlx->root->node : dlx->columns[j+1].node;
        curr_col->node->left  = (j == 0) ? dlx->root->node : dlx->columns[j-1].node;
        curr_col->node->colID = j;
        curr_col->node->rowID = -1;
        curr_col->node->column = curr_col;
        helper_nodes_cols[j] = curr_col->node;
    }

    dlx->root->node->right = dlx->columns[0].node;
    dlx->columns[0].node->left = dlx->root->node;
    dlx->root->node->left = dlx->columns[cols-1].node;
    dlx->columns[cols-1].node->right = dlx->root->node;


    
    //we add one extra row for root nodes
    for(int  i =0 ; i<rows;i++){
        helper_nodes_rows[i] = NULL;
        first_in_row[i] = NULL;
        for(int j =0 ;j <cols;j++){
            if(get_value_array_2d(problem_matrix,i,j)){
                dlx->columns[j].size++;
                Node *new_node = (Node*)malloc(sizeof(Node));
                new_node->colID = j;
                new_node->rowID = i;
                new_node->column = &dlx->columns[j];
                new_node->down = NULL;
                new_node->up = helper_nodes_cols[j];
                helper_nodes_cols[j]->down = new_node;
                helper_nodes_cols[j] = new_node;

                if(first_in_row[i] == NULL){
                    first_in_row[i] = new_node;
                    
                }
                else{
                    new_node->left = helper_nodes_rows[i];
                    helper_nodes_rows[i]->right = new_node;
                }
                helper_nodes_rows[i] = new_node;
            }
            
        }
    }
    for(int i =0 ; i < rows;i++){
        if(first_in_row[i]){
            first_in_row[i]->left = helper_nodes_rows[i];
            helper_nodes_rows[i]->right = first_in_row[i];
        }

    }
    for(int j =0 ; j <cols;j++){
        helper_nodes_cols[j]->down = dlx->columns[j].node;
        dlx->columns[j].node->up = helper_nodes_cols[j];
    }

    free(helper_nodes_cols);
    free(helper_nodes_rows);
    free(first_in_row);
    return dlx;
}

void cover_column(DLX *dlx,Node *node){
    int col = node->colID;
    dlx->columns[col].node->left->right = dlx->columns[col].node->right;
    dlx->columns[col].node->right->left = dlx->columns[col].node->left;
}


void cover(DLX* dlx,Node *node){
    ColNode* col_to_cover = &dlx->columns[node->colID];

    cover_column(dlx,col_to_cover->node);
    //Cover rows
    Node* row = col_to_cover->node->down;
    while (row != col_to_cover->node) {
        Node *right = row->right;
        while (right != row) {
            right->up->down = right->down;
            right->down->up = right->up;
            dlx->columns[right->colID].size--;
            right = right->right;
        }
        row = row->down;
    }
}

void uncover_column(DLX* dlx,Node *node){
    int col = node->colID;
    dlx->columns[col].node->left->right = dlx->columns[col].node;
    dlx->columns[col].node->right->left = dlx->columns[col].node;
}

void uncover(DLX *dlx, Node *node){
    
    ColNode *col = &dlx->columns[node->colID];
    Node* row = col->node->down;
    while(row != col->node){
        Node *left = row->left;
        while (left != row)
        {
            dlx->columns[left->colID].size++;
            left->up->down = left;
            left->down->up = left;
            left = left->left;
        }
       row = row->up;
    }
    uncover_column(dlx,col->node);
}

int isEmpty(DLX *dlx){
    if(dlx->root->node->right == dlx->root->node) return 1;
    return 0;
}

ColNode* get_min_col(DLX *dlx){
    printf("in get_min_col  ");
    Node *curr = dlx->root->node->right , *min_node = curr;
    if(!curr){
        printf("NULL CURR");
    }
    int min = INT_MAX;
    while(curr != dlx->root->node){
        
        int curr_size = dlx->columns[curr->colID].size;
        printf("col ID %d size_of_col %d \n", curr->colID , curr_size);
        if(curr_size < min) {
            min =curr_size;
            min_node = curr;
        }
        curr = curr->right;
        if(!curr){
            printf("NULL CURR  ");
        }
    }
    printf("min col id %d",min_node->colID);
    return &dlx->columns[min_node->colID];
}


Stack* rec(DLX *dlx , Stack *partial_solution, ListOfLists *solutions){
    printf("depth\n");
    if(isEmpty(dlx)){
        from_stack_to_ListOfLists(partial_solution,solutions);
        return partial_solution;
    }

    ColNode *min_col = get_min_col(dlx);
    if(min_col->size == 0) return NULL;
    Node *curr_node = min_col->node->down;
    while(curr_node != min_col->node){
        printf("in the loop\n");
        push_stack(partial_solution,curr_node);
        Node *right = curr_node->right;
        cover(dlx,curr_node);
        while (right != curr_node) {
            cover(dlx, right);
            right = right->right;
        }
        Stack *solution_val = rec(dlx,partial_solution,solutions);
        Node *left = curr_node->left;
        while (left != curr_node) {
            uncover(dlx, left);
            left = left->left;
        }
        uncover(dlx,curr_node);
        pop_stack(partial_solution);
        curr_node = curr_node->down;
    }
    return NULL;
}

ListOfLists* solve_dlx(DLX*dlx){
    Stack *partial = init_stack();
    ListOfLists *solutions = init_ListOfLists();
    rec(dlx,partial , solutions);
    return solutions;
}


void free_DLX(DLX *dlx){
    free(dlx->columns);
    free(dlx->solution);
}


int index_cell_contains(int col,int row,int grid_size){
    return row * grid_size + col;
}

int index_row_digit(int row,int num,int grid_size){
    return grid_size * grid_size + row * grid_size + num-1;
}

int index_col_digit(int col,int num,int grid_size){
    return grid_size * grid_size *2 + col * grid_size +num-1;
}

int index_box_digit(int box,int num,int grid_size){
    return grid_size * grid_size *3 + box * grid_size +num-1;
}


void set_up_dlx_matrix(Array2D *matrix,int i,int j,int grid_size,int num,int counter){
    set_value_array_2d(matrix,counter,index_cell_contains(i,j,grid_size),1);
    set_value_array_2d(matrix,counter,index_row_digit(i,num,grid_size),1);
    set_value_array_2d(matrix,counter,index_col_digit(j,num,grid_size),1);
    set_value_array_2d(matrix,counter,index_box_digit(box_id_grid(grid_size,i,j),num,grid_size),1);
}


//public
Array2D* transform_grid_to_DLX_naive(grid *g){
    int n = g->grid_size;
    int cols = 4 * n *n;
    int rows =  n*n + g->to_find *(n-1);
    Array2D *dlx_matrix = init_array_2d(rows,cols);
    int number_of_row = 0;
    for(int i =0;i < n; i++){
        for(int  j = 0;j<n;j++){
            int curr_val = get_value_array_2d(g->array,i,j);
            if(curr_val){
                set_up_dlx_matrix(dlx_matrix,i,j,n,curr_val,number_of_row);
                number_of_row++;
            }
            else{
                for(int k =0 ; k < n;k++){
                    set_up_dlx_matrix(dlx_matrix,i,j,n,k+1,number_of_row);
                    number_of_row++;
                }
            }
        }
    }
    return dlx_matrix;
}

void from_dlx_data_to_array_cords(int *arr, int grid_size , int* res_arr){
    //So we return a pointer to arr where arr is [row,col,number] format
    for(int i =0 ; i <3 ;i++){
        if( *arr <  grid_size * grid_size){
            res_arr[0] = *arr / grid_size;
            res_arr[1] = *arr % grid_size;
        }
        if(grid_size * grid_size <= *arr &&  *arr < 2 * grid_size * grid_size){
            res_arr[0] = (*arr - grid_size * grid_size) / grid_size;
            res_arr[2] = (*arr - grid_size * grid_size) % grid_size +1;
        }
    }
    return;

}

Array2D* from_list_of_lists_to_array(DLX *dlx,ListOfLists *solutions,int solution_index, int grid_size){
    if (!dlx || !solutions || solution_index < 0 || solution_index >= solutions->size)
        return NULL;
    Array2D *grid_matrix = init_array_2d(grid_size,grid_size);
    
    ListOfListsNode *curr = solutions->head;
    for(int i =0 ; i < solution_index && curr;i++){
        curr = curr->next;
    }
    if (!curr) return NULL;
    List *list = curr->data;
    ListNode *curr_list_node = list->head;
    Node *curr_row;
    while(curr_list_node){
        curr_row = curr_list_node->data;
        Node *curr_node_in_row = curr_row;
        int arr[3] ,res_arr[3] ,next_index = 0;
        do {
            if(curr_node_in_row){
                arr[next_index] =curr_node_in_row->colID;
                next_index++;
            }
            curr_node_in_row = curr_node_in_row->right;
        }
        while(curr_node_in_row != curr_row);
        from_dlx_data_to_array_cords(arr,dlx->cols,res_arr);
        set_value_array_2d(grid_matrix,res_arr[0],res_arr[1],res_arr[2]);
        curr_list_node = curr_list_node->next;
    }
    return grid_matrix;
}


int number_of_solutions(ListOfLists *list){
    return list->size;
}

