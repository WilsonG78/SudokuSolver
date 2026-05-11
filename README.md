# SudokuSolver

A full-stack Sudoku solving web application. The user enters a puzzle in the browser, selects a solving algorithm, and receives the solution instantly. The heavy lifting is done by a high-performance C engine exposed to Python via Pybind11, served through a Flask REST API, and displayed in a React/TypeScript frontend.

---

## Architecture

```
Browser (React/TS)
      │  POST /api/submit_grid
      ▼
Flask API (Python)
      │  Grid class + NumPy
      ▼
CSudokuSolver (Pybind11)
      │
      ▼
C solver core
  ├── DFS  – recursive backtracking
  └── DLX  – Knuth's Algorithm X with Dancing Links
```

| Layer | Tech |
|---|---|
| Frontend | React 18, TypeScript, Vite |
| Backend | Python 3.11, Flask, NumPy |
| Solver | C (DFS + DLX), Pybind11 bindings |
| Deployment | Docker, docker-compose |

---

## Algorithms

### DFS (Depth-First Search / Backtracking)
Classic recursive backtracking. Tries each candidate digit for each empty cell, backtracks on conflict. Always finds one solution if the puzzle is solvable. Simple and reliable, but slower on harder puzzles.

### DLX (Dancing Links X)
Implementation of **Donald Knuth's Algorithm X** using the Dancing Links technique. The Sudoku puzzle is reduced to an **exact cover problem** and solved with a sparse doubly-linked list structure that allows efficient backtracking. Significantly faster than DFS on difficult puzzles and can enumerate all solutions.

---

## Quick Start (Docker)

```bash
git clone https://github.com/WilsonG78/SudokuSolver.git
cd SudokuSolver
docker-compose up --build
```

| Service | URL |
|---|---|
| Frontend | http://localhost:8080 |
| Backend API | http://localhost:5000 |

---

## Local Development

### Backend

**Requirements:** Python 3.11+, GCC / Clang, cmake

```bash
cd backend
pip install -r requirements.txt
pip install .          # compiles the C extension via Pybind11
python run.py          # starts Flask on port 5000
```

### Frontend

**Requirements:** Node 18+

```bash
cd frontend
npm install
npm run dev            # starts Vite dev server on port 5173
```

Make sure `frontend/.env` points to your running backend:

```
VITE_BACKEND_URL=http://localhost:5000
```

---

## API

### `POST /api/submit_grid`

Solve a Sudoku puzzle.

**Request body**
```json
{
  "grid": [["5","3","","","7","","","",""], ...],
  "solver": "DLX"
}
```

Empty cells are represented as `""`. `solver` is either `"DLX"` or `"DFS"`.

**Response**
```json
{
  "status": "success",
  "solver": "DLX",
  "solution_count": 1,
  "solutions": [[[5,3,4,6,7,8,9,1,2], ...]]
}
```

---

## Project Structure

```
SudokuSolver/
├── backend/
│   ├── sudoku_solver/
│   │   ├── data_structures_C/   # C implementation: DLX, DFS, grid, set, array_2d
│   │   ├── sudoku_bindings.cpp  # Pybind11 C++ <-> Python bridge
│   │   ├── c_solver.c / .h      # Top-level C solver entry points
│   │   └── grid.py              # Python Grid class wrapping the C solver
│   ├── server.py                # Flask REST API
│   ├── run.py                   # Production entry point (Waitress WSGI)
│   ├── setup.py                 # Builds the C extension
│   └── Dockerfile
├── frontend/
│   ├── src/
│   │   ├── SudokuGrid.tsx       # Interactive 9x9 grid with solver selection
│   │   └── App.tsx
│   └── Dockerfile
├── tests/
│   └── test_grid.py
├── sudokus/                     # Sample puzzle files
└── docker-compose.yml
```

---

## Running Tests

```bash
# from project root
pip install -e backend
pytest tests/
```

Test data sourced from the [sudoku-exchange-puzzle-bank](https://github.com/grantm/sudoku-exchange-puzzle-bank) by grantm.

---

## Puzzle File Format

Plain text, space-separated digits, `0` for empty cells:

```
5 3 0 0 7 0 0 0 0
6 0 0 1 9 5 0 0 0
0 9 8 0 0 0 0 6 0
...
```

Pass a file path to `Grid(file_name="sudokus/1sudoku.txt")` to load it programmatically.