#include "grid.h"

grid* init_grid(int **arr, int size){
    Array2D *new_array = init_array_2d(size,size);

    //Init sets Sets contains every posible value in this row or col or box
    set *new_rows = malloc(size * sizeof(set));
    set *new_cols = malloc(size * sizeof(set));
    set *new_boxes = malloc(size *(sizeof(set)));
    for(int i =0 ; i < size; i++){
        init_set(new_rows +i);
        init_set(new_cols +i);
        init_set(new_boxes +i);
        for(int j =0 ; j< size; j++) {
            insert_set(new_rows +i , j+1);
            insert_set(new_cols +i , j+1);
            insert_set(new_boxes +i , j+1);
        }
    }
    int new_to_find = 0;

    for(int i = 0; i <size; i++){
        for(int j =0 ; j < size; j++){
            int val = arr[i][j];
            set_value_array_2d(new_array,i,j,val);
            if( val != 0){
                new_to_find++;
                delete_set(new_rows+i,val);
                delete_set(new_cols + j,val);
                delete_set(new_boxes+ box_id_grid(size,i,j),val);
            }
        }
    }
    grid *new_grid = (grid*)malloc(sizeof(grid));
    new_grid->array =new_array;
    new_grid->rows = new_rows;
    new_grid->cols = new_cols;
    new_grid->boxes = new_boxes;
    new_grid->grid_size = size;  
    new_grid->to_find = new_to_find;
    return new_grid;
}


void free_grid(grid *g){
    free_array_2d(g->array);
    for(int i =0 ; i < g->grid_size;i++){
        free_set(g->rows +i);
        free_set(g->cols +i);
        free_set(g->boxes +i);
    }
    free(g->rows);
    free(g->cols);
    free(g->boxes);
    free(g);
}

static int num_digits(int x) {
    int count = 1;
    if (x < 0) x = -x; 
    while (x >= 10) {
        x /= 10;
        count++;
    }
    return count;
}

void print_hline(int *cell_w,int n) {
    int b = (int)sqrt(n);
    for (int block = 0; block < n; block += b) {
        printf("+");
        for (int k = 0; k < (*cell_w + 1) * b + 1; k++) { 
            printf("-");
        }
    }
    printf("+\n");
}

void print_grid(grid *g) {
    int n = g->grid_size;
    int b = (int)sqrt(n); 
    printf("\n");

    
    int cell_w = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int d = num_digits(get_value_array_2d(g->array,i,j));
            if (d > cell_w) cell_w = d;
        }
    }

    

    
    print_hline(&cell_w,n);
    for (int i = 0; i < n; i++) {
        for (int start = 0; start < n; start += b) {
            printf("|");
            for (int j = start; j < start + b; j++) {
                
                printf(" %*d", cell_w,get_value_array_2d(g->array,i,j));
            }
            printf(" ");
        }
        printf("|\n");
        if ((i + 1) % b == 0) {
            print_hline(&cell_w,n);
        }
    }
}




int box_id_grid(int size, int i , int j ){
    int sub_n = (int)sqrt(size);
    return (i /sub_n) * sub_n + j/sub_n;
}



int equal_grid(grid *grid_a , grid* grid_b){
    if(grid_a->grid_size != grid_b->grid_size) return 0;
    int n = grid_a->grid_size;
    for(int i =0 ;i <n;++i){
        for(int  j =0 ; j< n;++j){
            if(get_value_array_2d(grid_a->array,i,j) != get_value_array_2d(grid_b->array,i,j)){
                return 0;
            }
        }
    }
    return 1;
}