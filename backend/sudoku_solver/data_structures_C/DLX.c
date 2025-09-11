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
        Node *row_node = curr->data;
        push_node(new_list,row_node);
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
    if(!dlx){
        perror("Dlx matrix is not allocated");
    }
    int cols = problem_matrix->cols, rows = problem_matrix->rows;
    dlx->cols = cols;
    dlx->rows = rows;
    dlx->solution = (Node **)malloc(rows * sizeof(Node*));
    dlx->solutionSize = 0;
    
    //root node
    dlx->root = (ColNode *)malloc(sizeof(ColNode));
    dlx->root->node = (Node*)malloc(sizeof(Node));
    dlx->root->node->colID = -1;
    dlx->root->node->rowID = -1;
    dlx->root->node->column = dlx->root;
    dlx->root->size = 0;

    //it is gonna be fixed
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

    for (int j = 0; j < cols; j++) {
        dlx->columns[j].node = (Node*)malloc(sizeof(Node));
        ColNode *curr_col = &dlx->columns[j];
        curr_col->size = 0;

        curr_col->node->colID = j;
        curr_col->node->rowID = -1;
        curr_col->node->column = curr_col;

        // init as vertical cycle
        curr_col->node->up = curr_col->node;
        curr_col->node->down = curr_col->node;
    }

    // link all columns in a doubly-linked horizontal cycle with root
    Node *prev = dlx->root->node;
    for (int j = 0; j < cols; j++) {
        Node *col_node = dlx->columns[j].node;

        prev->right = col_node;
        col_node->left = prev;

        prev = col_node;
    }
    // close the cycle
    prev->right = dlx->root->node;
    dlx->root->node->left = prev;

    for(int j=0 ; j < cols;j++){
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

static inline void cover_column_node(Node *col_node) {
    col_node->right->left = col_node->left;
    col_node->left->right = col_node->right;
}

static inline void uncover_column_node(Node *col_node) {
    col_node->left->right = col_node;
    col_node->right->left = col_node;
}



void cover(DLX *dlx, Node *col_node) {
    if (!dlx || !col_node) return;
    // remove column header from header list
    cover_column_node(col_node);

    // for each row i in column c (downwards)
    for (Node *i = col_node->down; i != col_node; i = i->down) {
        // for each node j in row i (rightwards)
        for (Node *j = i->right; j != i; j = j->right) {
            // unlink j vertically from its column
            j->up->down = j->down;
            j->down->up = j->up;
            // decrement column size
            dlx->columns[j->colID].size--;
        }
    }
}


void uncover(DLX *dlx, Node *col_node) {
    if (!dlx || !col_node) return;

    // for each row i in column c, going upwards (reverse order)
    for (Node *i = col_node->up; i != col_node; i = i->up) {
        // for each node j in row i going leftwards (reverse order)
        for (Node *j = i->left; j != i; j = j->left) {
            // restore vertical links
            dlx->columns[j->colID].size++;
            j->up->down = j;
            j->down->up = j;
        }
    }

    // restore column header into header list
    uncover_column_node(col_node);
}

int isEmpty(DLX *dlx){
    if(dlx->root->node->right == dlx->root->node) return 1;
    return 0;
}

ColNode* get_min_col(DLX *dlx) {
    if (!dlx || !dlx->root || !dlx->root->node) return NULL;
    Node *curr = dlx->root->node->right;
    if (curr == dlx->root->node) return NULL; 

    Node *min_node = curr;
    int min = INT_MAX;
    for (; curr != dlx->root->node; curr = curr->right) {
        int s = dlx->columns[curr->colID].size;
        if (s < min) {
            min = s;
            min_node = curr;
            if (min == 0) break;
        }
    }
    return &dlx->columns[min_node->colID];
}

void rec(DLX *dlx , Stack *partial_solution, ListOfLists *solutions){
    if(isEmpty(dlx)){
        from_stack_to_ListOfLists(partial_solution,solutions);
        return;
    }

    ColNode *min_col = get_min_col(dlx);
    if(min_col->size == 0) return;
    for (Node *r = min_col->node->down; r != min_col->node; /* r updated inside */) {
        Node *next_row = r->down; 

        push_stack(partial_solution, r);

        // cover column c
        cover(dlx, min_col->node);
        
        // cover columns for each node in row r
        for (Node *j = r->right; j != r; j = j->right) {
            cover(dlx, j->column->node);
        }

        rec(dlx, partial_solution, solutions);

        // uncover
        for (Node *j = r->left; j != r; j = j->left) {
            uncover(dlx, j->column->node);
        }
        uncover(dlx, min_col->node);

        pop_stack(partial_solution);

        r = next_row;
    }
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


int index_cell_contains(int row,int col,int grid_size){
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
    int rows = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            int val = get_value_array_2d(g->array, i, j);
            if(val) 
                rows += 1;   
            else 
                rows += n;   
        }
    }

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
        if( arr[i] <  grid_size * grid_size){
            res_arr[0] = arr[i] / grid_size;
            res_arr[1] = arr[i] % grid_size;
        }
        if(arr[i] &&  arr[i] < 2 * grid_size * grid_size){
            res_arr[0] = (arr[i] - grid_size * grid_size) / grid_size;
            res_arr[2] = (arr[i] - grid_size * grid_size) % grid_size +1;
        }
    }
    return;

}

Array2D* from_list_of_lists_to_array(DLX *dlx, ListOfLists *solutions, int solution_index, int grid_size){
    if(!dlx || !solutions || solution_index < 0 || solution_index >= solutions->size)
        return NULL;

    Array2D *grid_matrix = init_array_2d(grid_size, grid_size);

    // Navigate to the desired solution
    ListOfListsNode *curr = solutions->head;
    for(int i = 0; i < solution_index && curr; i++)
        curr = curr->next;
    if(!curr) return NULL;

    List *solution = curr->data;
    ListNode *node_in_solution = solution->head;

    while(node_in_solution){
        Node *row_head = node_in_solution->data;
        Node *n = row_head;
        int row = -1, col = -1, num = -1;

        do {
            int c = n->colID;

            if(c < grid_size * grid_size){       // cell constraint
                row = c / grid_size;
                col = c % grid_size;
            } else if(c < 2 * grid_size * grid_size){ // row constraint
                num = (c - grid_size * grid_size) % grid_size + 1;
            } else if(c < 3 * grid_size * grid_size){ // column constraint (optional safety)
                num = (c - 2 * grid_size * grid_size) % grid_size + 1;
            }

            n = n->right;
        } while(n != row_head);

        if(row >= 0 && col >= 0 && num > 0){
            set_value_array_2d(grid_matrix, row, col, num);
        }

        node_in_solution = node_in_solution->next;
    }

    return grid_matrix;
}


int number_of_solutions(ListOfLists *list){
    return list->size;
}