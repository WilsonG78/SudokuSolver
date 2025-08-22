#include "set.h"
#include "array_2d.h"
#include <math.h>

typedef struct grid{
    Array2D *array;
    Array2D *orginal_array;
    set *rows;
    set *cols;
    set *boxes;
    int grid_size;
    int to_find;
}grid;

grid* init_grid(int **arr, int size);
void free_grid(grid *g);
int box_id_grid(int size, int i ,int j);
void print_grid(grid *g);