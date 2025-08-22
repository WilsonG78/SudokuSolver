#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int **read_and_validate_sudoku(const char *filename ,int *n);
int validate_sudoku_with_box_id(int **grid , int n);