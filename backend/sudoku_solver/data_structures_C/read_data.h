#ifndef READ_DATA_H
#define READ_DATA_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int **read_and_validate_sudoku(const char *filename ,int *n);
int validate_sudoku_with_box_id(int **grid , int n);
int **read_sudoku_exchange_bank(char* filename, int *n, int line_number);

#endif READ_DATA_H