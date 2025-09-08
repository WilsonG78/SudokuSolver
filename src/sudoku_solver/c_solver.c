#include <stdlib.h>
#include <stdio.h>
#include "data_structures_C/set.h"
#include "data_structures_C/array_2d.h"
#include "data_structures_C/grid.h"
#include "data_structures_C/read_data.h"
#include "data_structures_C/DLX.h"


int solve_dfs_sudoku_c(grid *g, int row, int col) {
    //printf("I am here i: %d j: %d\n");
    if (row == g->grid_size) return 1;

    int next_row = (col == g->grid_size - 1) ? row + 1 : row;
    int next_col = (col + 1) % g->grid_size;

    if (get_value_array_2d(g->array, row, col) != 0) {
        return solve_dfs_sudoku_c(g, next_row, next_col);
    }

    for (int num = 1; num <= g->grid_size; num++) {
        if (contains_set(g->rows + row, num) &&
            contains_set(g->cols + col, num) &&
            contains_set(g->boxes + box_id_grid(g->grid_size, row, col), num)) {

            set_value_array_2d(g->array, row, col, num);
            delete_set(g->rows + row, num);
            delete_set(g->cols + col, num);
            delete_set(g->boxes + box_id_grid(g->grid_size, row, col), num);

            if (solve_dfs_sudoku_c(g, next_row, next_col)) return 1;

            
            set_value_array_2d(g->array, row, col, 0);
            insert_set(g->rows + row, num);
            insert_set(g->cols + col, num);
            insert_set(g->boxes + box_id_grid(g->grid_size, row, col), num);
        }
    }

    return 0; 
}


int solve_dlx_naive_sudoku_c(grid *g){
    Array2D *dlx_matrix = transform_grid_to_DLX_naive(g);
    DLX *dlx = init_DLX(dlx_matrix);
    ListOfLists *solutions = solve_dlx(dlx);
    for(int i =0 ; i< solutions->size;i++){
        Array2D *new_array = from_list_of_lists_to_array(dlx,solutions,i,g->grid_size);
        grid *new_grid = init_grid(new_array->array,new_array->rows);
        print_grid(new_grid);
        free_grid(new_grid);
    }
    return 0;
}


int main(){
    char *file_name = "sudokus/1sudoku.txt";
    int size;
    int **arr = read_and_validate_sudoku(file_name,&size);
    grid* main_grid = init_grid(arr,size);
    print_grid(main_grid);
    printf("dfs solution\n");
    solve_dfs_sudoku_c(main_grid,0,0);
    printf("here is DLX solution\n");
    solve_dlx_naive_sudoku_c(main_grid);
    print_grid(main_grid);
    free_grid(main_grid);

}