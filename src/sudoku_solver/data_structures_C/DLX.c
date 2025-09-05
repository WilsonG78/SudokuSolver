#include <DLX.h>


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
    if(!stack){
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

Node** pop_stack(Stack *stack){
    if(!stack || !stack->tail){
        return NULL;
    }
    
    Vertex *to_del = stack->tail;
    Node **to_return = to_del->data;
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
            push_node(new_list,node->rowID);
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
    dlx->root->node->column = &dlx->root;
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
        curr_col->node->down =NULL;
        curr_col->node->up = NULL;
        curr_col->size = 0;
        curr_col->node->right = dlx->columns[(j+1) % cols].node;
        curr_col->node->left = dlx->columns[(j-1) % cols].node;
        curr_col->node->colID = j;
        curr_col->node->rowID = -1;
        curr_col->node->column = &curr_col;
        helper_nodes_cols[j] = curr_col->node;
    }

    dlx->root->node->right = &dlx->columns[0].node;
    dlx->columns[0].node->left = dlx->root->node;
    dlx->root->node->left = &dlx->columns[cols-1].node;
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
            helper_nodes_rows[i]->right = first_in_row;
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

    //Cover rows
    Node* curr_node = col_to_cover->node->down;
    while(curr_node != col_to_cover->node){
        Node *curr_row_node = curr_node->right;
        while (curr_node != curr_row_node)
        {
            curr_row_node->up->down = curr_row_node->down;
            curr_row_node->down->up = curr_row_node->up;
            dlx->columns[curr_row_node->colID].size--;
        }
        cover_column(dlx,curr_row_node);
    }
}

void uncover_column(DLX* dlx,Node *node){
    int col = node->colID;
    dlx->columns[col].node->left->right = dlx->columns[col].node;
    dlx->columns[col].node->right->left = dlx->columns[col].node;
}

void uncover(DLX *dlx, Node *node){
    
    ColNode *uncover_col = &dlx->columns[node->colID];
    Node* curr_node = uncover_col->node->down;
    while(curr_node != uncover_col->node){
        Node *curr_row_node = curr_node->right;
        while (curr_node != curr_row_node)
        {
            curr_row_node->up->down = curr_row_node;
            curr_row_node->down->up = curr_row_node;
            dlx->columns[curr_row_node->colID].size++;
        }
    }
}

int isEmpty(DLX *dlx){
    if(dlx->root->node->right == dlx->root->node) return 1;
    return 0;
}

ColNode* get_min_col(DLX *dlx){
    Node *curr = dlx->root->node->right , *min_node = curr;
    int min = -1;
    while(curr != dlx->root->node){
        int curr_size = dlx->columns[curr->colID].size;
        if(curr_size < min) {
            min =curr_size;
            min_node = curr;
        }
    }
    return &dlx->columns[min_node->colID];
}


Stack* rec(DLX *dlx , Stack *partial_solution, ListOfLists *solutions){
    if(isEmpty(dlx)) return partial_solution;
    ColNode *min_col = get_min_col(dlx);
    if(min_col->size == 0) return NULL;
    Node *curr_node = min_col->node->down;
    while(curr_node != min_col->node){
        push_stack(partial_solution,curr_node);
        cover(dlx,curr_node);
        Stack *solution_val = rec(dlx,partial_solution,solutions);
        if(solution_val){
            from_stack_to_ListOfLists(partial_solution , solutions);
        }
        pop_stack(partial_solution);
        uncover(dlx,curr_node);
    }
}

ListOfLists* solve_dlx(DLX*dlx){
    Stack *partial;
    ListOfLists *solutions;
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
    int cols = 4 * g->grid_size * g->grid_size;
    int rows =  g->to_find * g->grid_size + g->grid_size - g->to_find;
    int n = g->grid_size;
    Array2D *dlx_matrix = init_array_2d(rows,cols);
    int number_of_row = 0;
    for(int i;i < n; i++){
        for(int  j;j<n;j++){
            int curr_val = get_value_array_2d(g->array,i,j);
            if(curr_val){
                set_up_dlx_matrix(dlx_matrix,i,j,n,curr_val,number_of_row);
                number_of_row++;
            }
            for(int k =0 ; k < n;k++){
                set_up_dlx_matrix(dlx_matrix,i,j,n,k+1,number_of_row);
                number_of_row++;
            }
        }
    }
    return dlx_matrix;
}

grid* from_dlx_solved_to_grid(){
    //TODO
}


int number_of_solutions(ListOfLists *list){
    return list->size;
}

