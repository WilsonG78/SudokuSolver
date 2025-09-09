#include "c_solver.h"
void free_SolvedGrids(SolvedGrids *sg) {
    if (!sg) return;

    for (int i = 0; i < sg->size; i++) {
        if (sg->grids[i]) {
            free_grid(sg->grids[i]);
        }
    }

    free(sg->grids);
    free(sg);
}


int dfs_rec(grid *g, int row, int col) {
    //printf("I am here i: %d j: %d\n");
    if (row == g->grid_size) return 1;

    int next_row = (col == g->grid_size - 1) ? row + 1 : row;
    int next_col = (col + 1) % g->grid_size;

    if (get_value_array_2d(g->array, row, col) != 0) {
        return dfs_rec(g, next_row, next_col);
    }

    for (int num = 1; num <= g->grid_size; num++) {
        if (contains_set(g->rows + row, num) &&
            contains_set(g->cols + col, num) &&
            contains_set(g->boxes + box_id_grid(g->grid_size, row, col), num)) {

            set_value_array_2d(g->array, row, col, num);
            delete_set(g->rows + row, num);
            delete_set(g->cols + col, num);
            delete_set(g->boxes + box_id_grid(g->grid_size, row, col), num);

            if (dfs_rec(g, next_row, next_col)) return 1;

            
            set_value_array_2d(g->array, row, col, 0);
            insert_set(g->rows + row, num);
            insert_set(g->cols + col, num);
            insert_set(g->boxes + box_id_grid(g->grid_size, row, col), num);
        }
    }

    return 0; 
}


SolvedGrids* solve_dfs_sudoku_c(grid *g){
    grid *new_grid = init_grid(g->array->array,g->grid_size);
    dfs_rec(new_grid,0,0);
    SolvedGrids* to_return = (SolvedGrids*)malloc(sizeof(SolvedGrids));
    grid **p_grid = &new_grid;
    if (!equal_grid(g,new_grid)){
        to_return->grids = (grid**)malloc(sizeof(grid*));
        to_return->size = 1;
        to_return->grids[0] = new_grid;
    }
    else{
        to_return->size = 0;
        to_return->grids = NULL;
    }
    
    return to_return;
}


SolvedGrids *solve_dlx_naive_sudoku_c(grid *g){
    Array2D *dlx_matrix = transform_grid_to_DLX_naive(g);
    DLX *dlx = init_DLX(dlx_matrix);
    ListOfLists *solutions = solve_dlx(dlx);
    grid **table = (grid**)malloc(sizeof(grid*) * solutions->size);
    
    for(int i =0 ; i< solutions->size;i++){
        Array2D *new_array = from_list_of_lists_to_array(dlx,solutions,i,g->grid_size);
        grid *new_grid = init_grid(new_array->array,new_array->rows);
        table[i] = new_grid;
        free_array_2d(new_array);
    }
    SolvedGrids *to_return = (SolvedGrids*)malloc(sizeof(SolvedGrids));
    to_return->grids = table;
    to_return->size = solutions->size;
    free_ListOfLists(solutions);
    free_DLX(dlx);
    free_array_2d(dlx_matrix);
    return to_return;
}


int main(){
    
    char *file_name = "sudokus/1sudoku.txt";
    int size;
    int **arr = read_sudoku_exchange_bank("sudoku-exchange-puzzle-bank/easy.txt", &size, 100);
    grid* main_grid = init_grid(arr,size);
    print_grid(main_grid);    
    SolvedGrids *dfs = solve_dfs_sudoku_c(main_grid);
    SolvedGrids *dlx_solutions = solve_dlx_naive_sudoku_c(main_grid);
    printf("here is DLX solution\n");
    if(dlx_solutions->size > 0) print_grid(dlx_solutions->grids[0]);

    printf("dfs solution\n");
    if(dfs->size > 0) print_grid(dfs->grids[0]);
    free_grid(main_grid);
    for(int  i =0 ; i< size; i++){
        free(arr[i]);
    }
    free(arr);
    free_SolvedGrids(dfs);
    free_SolvedGrids(dlx_solutions);
}