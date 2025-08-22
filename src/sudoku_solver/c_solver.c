#include <stdlib.h>
#include "data_structures_C/set.h"
#include "data_structures_C/array_2d.h"
#include "data_structures_C/grid.h"
#include "data_structures_C/read_data.h"



int recurrsive_dfs(grid *g, Array2D *visited,int i,int j){
    if(g->to_find == 0) return 1;
    int dirs[][2]  = {{-1,0},{0,1},{0,-1},{1,0}};
    int res =0;
    set_value_array_2d(visited,i,j,1);
    if(!get_value_array_2d(g->orginal_array,i,j)){
        for(int k =0 ; k < 4;k++){
            int nx = dirs[k][1] + j, ny = dirs[k][0] + i;
            if(nx >= 0 && ny >= 0 && nx < g->grid_size && ny < g->grid_size && !get_value_array_2d(visited,ny,nx)){
                res |= recurrsive_dfs(g,visited,ny,nx);
            }
        }
    }
    else if(!get_value_array_2d(g->array,i,j)){
        int counter =0 ;
        g->to_find =0;
        for( int number =1 ; number < g->grid_size +1; number++) {
            if(contains_set(g->rows + i,number) && contains_set(g->rows +j , number) && contains_set(g->boxes + box_id_grid(g->grid_size,i,j),number)){
                counter++;
                for(int k =0 ; k < 4;k++){
                    int nx = dirs[k][1] + j, ny = dirs[k][0] + i;
                    if(nx >= 0 && ny >= 0 && nx < g->grid_size && ny < g->grid_size && !get_value_array_2d(visited,ny,nx)){
                        res |= recurrsive_dfs(g,visited,ny,nx);
                    }
                }
            }
        }
        if(!counter){
            return 0;
        }
        g->to_find +=1;
    }
    set_value_array_2d(visited,i,j,0);
}


int solve_dfs_c(grid *grid){
    if (!grid){
        fprintf(stderr , "Grid is NULL");
    }
    Array2D *visited = init_array_2d(grid->grid_size,grid->grid_size);
    if(recurrsive_dfs(grid,visited,0,0)){
        return 1;
    }
    return 0;
    
    
}


int main(){
    char *file_name = "../../sudokus/1sudoku.txt";
    int size;
    int **arr = read_and_validate_sudoku(file_name,&size);
    printf("%d",size);
    grid* main_grid = init_grid(arr,size);
    printf("%d",main_grid->grid_size);
    free_grid(main_grid);
}