#include <stdlib.h>
#include <stdio.h>
#include "array_2d.h"

Array2D* init_array_2d(int rows,int cols){
    int **new_array = malloc(rows * sizeof(int *));
    if(!new_array){
        perror("Failed to allocate memory for rows");
        return NULL;
    }
    for(int i =0 ; i <rows; i++){
        new_array[i] = calloc(cols, sizeof(int));
        if(!new_array[i]){
            perror("Failed to allocate memory for columns");
            for(int j =0 ; j < i; j++) free(new_array[j]);
            free(new_array);
            return NULL;
        }
    }
    Array2D* new_array_2d = (Array2D*)malloc(sizeof(Array2D));
    if(!new_array_2d){
        perror("Failed to allocate memory for Array2D structure");
        for (int i = 0; i < rows; i++) {
            free(new_array[i]);
        }
        free(new_array);
        return NULL;
    }
    new_array_2d->array = new_array;
    new_array_2d->rows = rows;
    new_array_2d->cols = cols;
    return new_array_2d;
}

void free_array_2d(Array2D *arr) {
    if (arr) {
        if (arr->array) {
            for (int i = 0; i < arr->rows; i++) {
                free(arr->array[i]);
            }
            free(arr->array);
        }
        free(arr);
    }
}


int get_value_array_2d(Array2D *arr, int i , int j){
    if(!arr || !arr->array){
        fprintf(stderr, "Array is not initialized\n");
        return -1;
    }
    if( i  < 0 || i >= arr->rows || j < 0 || j >= arr->cols){
        fprintf(stderr, "Invalid coordinates: i=%d, j=%d (rows=%d, cols=%d)\n", i, j, arr->rows, arr->cols);
        return -1;
    }
    return arr->array[i][j];
}

int set_value_array_2d(Array2D *arr , int i , int j, int val ) {
    if (!arr || !arr->array) {
        fprintf(stderr, "Array is not initialized\n");
        return -1;
    }
    if( i  < 0 || i >= arr->rows || j < 0 || j >= arr->cols){
        fprintf(stderr, "Invalid coordinates: i=%d, j=%d (rows=%d, cols=%d)\n", i, j, arr->rows, arr->cols);
        return -1;
    }
    arr->array[i][j] = val;
    return 1;
}

void set_from_arr_array_2d(Array2D *array_2d ,int **arr , int rows,int cols) {
    if(!array_2d || !array_2d->array) return;
    if(array_2d->cols != cols || array_2d->rows != rows){
        fprintf(stderr, "Dimension mismatch in set_from_arr_array_2d\n");
        return;
    }
    for(int i =0 ; i < rows; i++){
        for (int j =0 ; j <cols ; j++){
            array_2d->array[i][j] = arr[i][j];
        }
    }
}