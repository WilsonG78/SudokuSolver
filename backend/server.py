from flask import Flask
from flask_cors import CORS
from flask import request, jsonify
from sudoku_solver import Grid
import numpy as np
app = Flask(__name__)
CORS(app)

@app.route('/api/submit_grid' ,methods = ["POST"])
def submit_grid():
    try:
        data = request.get_json()
        grid = data.get('grid')
        solver = data.get('solver')

        print(f"Grid: {grid}")
        print(f"Solver {solver}")

        grids = solve(grid,solver)
        list_of_grids = [solution.tolist() for solution in grids]
        for grid in grids:
            newGrid = Grid(grid)
            print(newGrid)
        return jsonify({
                "status": "success", 
                "message": "Grid received",
                "solver": solver,
                "solutions": list_of_grids,
                "solution_count": len(grids)
            })
    except Exception as e:
        print(f"Error: {e}")
        return jsonify({"status": "not success",
                       "error": str(e)})

def list_to_np(list_grid: list[list[str]])->np.array:
    list_grid = [[0 if col == '' else int(col) for col in row] for row in list_grid]
    arr = np.array(list_grid)
    return arr

def solve(list_grid: list[list[str]],solver:str):
    arr = list_to_np(list_grid)
    new_grid = Grid(arr)
    grids = None
    if solver == "DLX":
        grids = new_grid.solve_dlx_np()
    elif solver == "DFS":
        grids = new_grid.solve_dfs_np()
    return grids
    




if __name__ == "__main__":
    app.run(host = "0.0.0.0", port =5000,debug= True)