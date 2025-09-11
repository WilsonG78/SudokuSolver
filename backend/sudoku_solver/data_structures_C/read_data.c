#include "read_data.h"


int **read_and_validate_sudoku(const char *filename ,int *n){
    FILE *file =fopen(filename ,"r");
    if(!file){
        perror("Error opening file");
        return NULL;
    }

    char buffer[1024];
    int rows = 0 , cols = 0, first_line =1;
    while(fgets(buffer,sizeof(buffer),file)){
        buffer[strcspn(buffer,"\n")] = '\0';
        if(buffer[0] == '\0') continue;
        rows++;
        int current_cols = 0;
        char *token = strtok(buffer, " \t");
        while(token){
            current_cols++;
            token = strtok(NULL, " \t");
        }
        if (first_line) {
            cols = current_cols;
            first_line = 0;
        } else if (current_cols != cols) {
            fprintf(stderr, "Error: inconsistent column count\n");
            fclose(file);
            return NULL;
        }
    }
    if (rows == 0 || cols == 0 || rows != cols) {
        fprintf(stderr, "Error: grid must be square and non-empty\n");
        fclose(file);
        return NULL;
    }
    *n = rows;
    int sub_n = (int)sqrt(*n);
    if (sub_n * sub_n != *n) {
        fprintf(stderr, "Error: grid size %d is not a perfect square\n", *n);
        fclose(file);
        return NULL;
    }
    rewind(file);
    int **grid = malloc(rows * sizeof(int *));
    if (!grid) {
        perror("malloc failed");
        fclose(file);
        return NULL;
    }
    for (int i = 0; i < rows; i++) {
        grid[i] = malloc(cols * sizeof(int));
        if (!grid[i]) {
            perror("malloc failed");
            for (int j = 0; j < i; j++) free(grid[j]);
            free(grid);
            fclose(file);
            return NULL;
        }
    }
    int r = 0;
    while (fgets(buffer, sizeof(buffer), file) && r < rows) {
        if (buffer[0] == '\0') continue; // skip empty
        char *token = strtok(buffer, " \t");
        int c = 0;
        while (token && c < cols) {
            grid[r][c++] = atoi(token);
            token = strtok(NULL, " \t");
        }
        if (c != cols) {
            fprintf(stderr, "Error: inconsistent number of columns in row %d\n", r);
            for (int i = 0; i <= r; i++) free(grid[i]);
            free(grid);
            fclose(file);
            return NULL;
        }
        r++;
    }
    
    fclose(file);

    if(!validate_sudoku_with_box_id(grid,*n)){
        for (int i = 0; i < rows; i++) free(grid[i]);
        free(grid);
        return NULL;
    }
    return grid;

}


int validate_sudoku_with_box_id(int **grid, int n) {
    int sub_n = (int)sqrt(n);
    if (sub_n * sub_n != n) return 0;

    
    int **row_seen = malloc(n * sizeof(int *));
    int **col_seen = malloc(n * sizeof(int *));
    int **box_seen = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        row_seen[i] = calloc(n, sizeof(int));
        col_seen[i] = calloc(n, sizeof(int));
        box_seen[i] = calloc(n, sizeof(int));
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int val = grid[i][j];
            if (val < 0 || val > n) goto invalid;
            if (val == 0) continue;

            int num_index = val - 1;
            int box_id = sub_n * (i / sub_n) + (j / sub_n);

            if (row_seen[i][num_index] || col_seen[j][num_index] || box_seen[box_id][num_index]) {
                printf("Duplicate %d found at row %d, col %d, box %d\n", val, i, j, box_id);
                goto invalid;
            }
            row_seen[i][num_index] = 1;
            col_seen[j][num_index] = 1;
            box_seen[box_id][num_index] = 1;
        }
    }

    
    for (int i = 0; i < n; i++) {
        free(row_seen[i]);
        free(col_seen[i]);
        free(box_seen[i]);
    }
    free(row_seen);
    free(col_seen);
    free(box_seen);

    return 1;

invalid:
    for (int i = 0; i < n; i++) {
        free(row_seen[i]);
        free(col_seen[i]);
        free(box_seen[i]);
    }
    free(row_seen);
    free(col_seen);
    free(box_seen);
    return 0;
}

int **read_sudoku_exchange_bank(char *filename, int *n, int line_number) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    *n = 9; // standard Sudoku

    int **grid = malloc(*n * sizeof(int*));
    if (!grid) {
        perror("malloc failed");
        fclose(file);
        return NULL;
    }
    for (int i = 0; i < *n; i++) {
        grid[i] = malloc(*n * sizeof(int));
        if (!grid[i]) {
            perror("malloc failed");
            for (int j = 0; j < i; j++) free(grid[j]);
            free(grid);
            fclose(file);
            return NULL;
        }
    }

    // Each line is exactly 100 bytes
    if (fseek(file, line_number * 101L, SEEK_SET) != 0) {
        fprintf(stderr, "Error seeking to line %d\n", line_number);
        goto error;
    }

    char line[101]; // 100 bytes + null terminator
    if (fread(line, 1, 100, file) != 100) {
        fprintf(stderr, "Error reading line %d\n", line_number);
        goto error;
    }
    line[100] = '\0';
    // Digits are at index 13..92 (81 characters)
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            char c = line[13 + i*9 + j];
            if (c >= '0' && c <= '9') {
                grid[i][j] = c - '0';
            } else {
                fprintf(stderr, "Invalid character '%c' in puzzle\n", c);
                goto error;
            }
        }
    }

    fclose(file);
    return grid;

error:
    for (int i = 0; i < *n; i++) free(grid[i]);
    free(grid);
    fclose(file);
    return NULL;
}