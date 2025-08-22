
#include <stdlib.h>
#include <stdio.h>

typedef struct Array2D {
    int **array;
    int rows;
    int cols;
} Array2D;

Array2D* init_array_2d(int rows, int cols);
void free_array_2d(Array2D *arr);
int get_value_array_2d(Array2D *arr, int i, int j);
int set_value_array_2d(Array2D *arr, int i, int j, int val);
void set_from_arr_array_2d(Array2D *array_2d , int **arr, int rows, int cols);

