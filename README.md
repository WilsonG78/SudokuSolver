# SudokuSolver
Sudoku Solver 

Sudoku Solver is a high-performance Sudoku solving application implemented in **C**. It currently support two solvers:
- **DFS (Depth-First-Search)** - a traditional backtracking solver which find always one solution if sudoku is solveable
- **DLX (Dancing Links X)** - an implentation of Knuth's efficient algorithm for exact problem


### Features

- Solve standard Sudoku puzzles automatically.  
- Supports multiple solving strategies (DLX).  
- High-performance C implementation for fast computation.  
- Can be integrated with Python via **Pybind11** for easy scripting.

### 1. Clone the repository

```bash
git clone https://github.com/WilsonG78/SudokuSolver.git
cd SudokuSolver
docker-compose up --build
```

This will build both the frontend and backend containers
Frontend will be accesible at 8080 port
Backend at 5000


I used public sudoku-exchange-puzzle-bank by grantm github link here https://github.com/grantm/sudoku-exchange-puzzle-bank for test cases