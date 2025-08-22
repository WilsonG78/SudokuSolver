import numpy as np
import os
import math

class Grid:
    grid: np.array
    grid_size: int
    box_size: int

    def __init__(self, array: np.array = None, file_name: str = None):
        if file_name is None and array is None:
            return
        if array is None:
            self.init_by_file(file_name)
            return
        self.init_by_array(array)

    def init_by_file(self, file_name: str) -> None:
        input_data = None
        with open(file_name) as f:
            input_data = f.read().strip()
        if not input_data:
            raise ValueError("File which should contain grid is empty")
        rows = input_data.split("\n")
        n = len(rows)
        self.grid_size = n
        
        try:
            grid_list = [list(map(int,row.split())) for row in rows]
        except Exception as e:
            raise ValueError(f"Error parsing number {e}")
        
        if any(len(row) != n for row in grid_list):
            raise ValueError(f"Invalid row lenght")
        if math.isqrt(n) **2 != n:
            raise ValueError(f"Invalid lenght of ")
        
        self.grid = np.array(grid_list , dtype =int)
        self.box_size = math.isqrt(n)
        if np.any(self.grid < 0) or np.any(self.grid > self.grid_size):
            raise ValueError(f"Numbers must be between 0 and {self.grid_size}")


    def init_by_array(self, array) -> None:
        if len(array.shape) != 2 or array.shape[0] != array.shape[1] or math.isqrt(array.shape[0])**2 != array.shape[0]:
            raise ValueError("Shape of array is not correct")
        self.grid_size = array.shape[0]
        for value in  array.flat:
            if value > self.grid_size or value < 0:
                raise ValueError(f"Bad number value to {value}")
        self.grid = array
        self.box_size = math.isqrt(self.grid_size)
        if np.any(self.grid < 0) or np.any(self.grid > self.grid_size):
            raise ValueError(f"Numbers must be between 0 and {self.grid_size}")

    def __str__(self):
        n = self.grid.shape[0]
        b = int(math.isqrt(n))
        
        cell_w = max(1, max(len(str(int(x))) for x in self.grid.flatten()))
        
        def format_row(row: np.ndarray) -> str:
            groups = []
            for start in range(0, n, b):
                cells = [str(int(v)).rjust(cell_w) for v in row[start:start+b]]
                groups.append(" " + " ".join(cells) + " ")
            return "|" + "|".join(groups) + "|"
        

        sample_row = format_row(np.zeros(n, dtype=int))
        hline = "".join('+' if ch == '|' else '-' for ch in sample_row)
        
        lines = [hline]
        for i in range(n):
            lines.append(format_row(self.grid[i]))
            if (i + 1) % b == 0:
                lines.append(hline)
        return "\n".join(lines)

        


if __name__ == "__main__":
    new_grid = Grid(file_name= '../sudokus/1sudoku.txt')
    print(new_grid)


