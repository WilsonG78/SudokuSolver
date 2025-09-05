import numpy as np

# Prefilled Sudoku
prefilled = {
    (0,0): 1,
    (1,2): 3,
    (2,1): 4,
    (3,3): 2
}

rows = []
for r in range(4):
    for c in range(4):
        if (r,c) in prefilled:
            digits = [prefilled[(r,c)]]  # only the given digit
        else:
            digits = [1,2,3,4]           # all possibilities

        for d in digits:
            row = [0]*64
            cell_col = r*4 + c
            row_col  = 16 + r*4 + (d-1)
            col_col  = 32 + c*4 + (d-1)
            box_col  = 48 + ((r//2)*2 + (c//2))*4 + (d-1)

            row[cell_col] = 1
            row[row_col]  = 1
            row[col_col]  = 1
            row[box_col]  = 1

            rows.append(row)

matrix = np.array(rows)

print("Matrix shape:", matrix.shape)
print(matrix)
