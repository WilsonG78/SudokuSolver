#include "grid.h"

grid* init_grid(int **arr, int size){
    Array2D *new_array = init_array_2d(size,size);
    Array2D *new_orginal_array = init_array_2d(size,size);

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
        }
    }
    int new_to_find = 0;

    for(int i = 0; i <size; i++){
        for(int j =0 ; j < size; j++){
            int val = arr[i][j];
            set_value_array_2d(new_array,i,j,val);
            set_value_array_2d(new_orginal_array,i,j,val);
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
    new_grid->orginal_array = new_orginal_array;
    new_grid->rows = new_rows;
    new_grid->cols = new_cols;
    new_grid->boxes = new_boxes;
    new_grid->grid_size = size;  
    new_grid->to_find = new_to_find;
    return new_grid;
}


void free_grid(grid *g){
    free_array_2d(g->array);
    free_array_2d(g->orginal_array);
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





int box_id_grid(int size, int i , int j ){
    int sub_n = (int)sqrt(size);
    return (i /sub_n) * sub_n + j/sub_n;
}