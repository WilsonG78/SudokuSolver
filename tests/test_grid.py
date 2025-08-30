import pytest
import os
import numpy as np
import math
from src.sudoku_solver.grid import Grid

def test_valid_grid_from_np_array():
    valid_arrays = [np.array([
    [5, 3, 0, 0, 7, 0, 0, 0, 0],
    [6, 0, 0, 1, 9, 5, 0, 0, 0],
    [0, 9, 8, 0, 0, 0, 0, 6, 0],
    [8, 0, 0, 0, 6, 0, 0, 0, 3],
    [4, 0, 0, 8, 0, 3, 0, 0, 1],
    [7, 0, 0, 0, 2, 0, 0, 0, 6],
    [0, 6, 0, 0, 0, 0, 2, 8, 0],
    [0, 0, 0, 4, 1, 9, 0, 0, 5],
    [0, 0, 0, 0, 8, 0, 0, 7, 9]
], dtype=int)]
    for array in valid_arrays:
        new_grid = Grid(array=array)
        assert new_grid is not None
        #assert new_grid.grid == new_grid
        assert new_grid.grid_size == array.shape[0]
        assert new_grid.box_size == math.isqrt(array.shape[0])

'''
def grid_from_file():
    for file_name in os.listdir('../sudokus'):
        new_grid = Grid(file_name= file_name)
        assert new_grid.grid.__str__()
        '''