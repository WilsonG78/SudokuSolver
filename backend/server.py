from flask import Flask
from flask_cors import CORS
from flask import request, jsonify
from sudoku_solver import Grid
import numpy as np
app = Flask(__name__)
CORS(app)

VALID_SOLVERS = {"DLX", "DFS"}

@app.route('/api/submit_grid' ,methods = ["POST"])
def submit_grid():
    try:
        data = request.get_json()
        grid = data.get('grid')
        solver = data.get('solver')

        if solver not in VALID_SOLVERS:
            return jsonify({"status": "error", "error": f"Unknown solver '{solver}'. Use 'DLX' or 'DFS'."}), 400

        grids = solve(grid, solver)
        list_of_grids = [solution.tolist() for solution in grids]
        for grid in grids:
            print(Grid(grid))
        return jsonify({
                "status": "success",
                "solver": solver,
                "solutions": list_of_grids,
                "solution_count": len(grids)
            })
    except Exception as e:
        print(f"Error: {e}")
        return jsonify({"status": "error", "error": str(e)}), 500

def list_to_np(list_grid: list[list[str]]) -> np.ndarray:
    list_grid = [[0 if col == '' else int(col) for col in row] for row in list_grid]
    return np.array(list_grid)

def solve(list_grid: list[list[str]], solver: str):
    arr = list_to_np(list_grid)
    new_grid = Grid(arr)
    if solver == "DLX":
        return new_grid.solve_dlx_np()
    return new_grid.solve_dfs_np()
    




if __name__ == "__main__":
    app.run(host = "0.0.0.0", port =5000,debug= True)