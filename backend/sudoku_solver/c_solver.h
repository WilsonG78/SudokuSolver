#ifndef C_SOLVER_H
#define C_SOLVER_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "data_structures_C/set.h"
#include "data_structures_C/array_2d.h"
#include "data_structures_C/grid.h"
#include "data_structures_C/read_data.h"
#include "data_structures_C/DLX.h"
#include "data_structures_C/ListOfLists.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SolvedGrids{
    int size;
    grid **grids;
}SolvedGrids;

void free_SolvedGrids(SolvedGrids *sg);
int dfs_rec(grid *g, int row, int col);
SolvedGrids* solve_dfs_sudoku_c(grid *g);
SolvedGrids* solve_dlx_naive_sudoku_c(grid *g);

#ifdef __cplusplus
}
#endif

#endif // C_SOLVER_H