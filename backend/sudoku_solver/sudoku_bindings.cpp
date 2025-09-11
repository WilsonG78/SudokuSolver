#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <vector>

extern "C" {
    #include "c_solver.h"
    #include "data_structures_C/grid.h"
    #include "data_structures_C/array_2d.h"
}

namespace py = pybind11;

// Convert a `grid*` to numpy array
py::array_t<int> grid_to_numpy(grid* g) {
    int n = g->grid_size;
    auto result = py::array_t<int>({n, n});
    auto buf = result.mutable_unchecked<2>();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            buf(i, j) = get_value_array_2d(g->array, i, j);
        }
    }
    return result;
}

// Convert NumPy array -> grid*
grid* numpy_to_grid(const py::array_t<int>& arr) {
    py::buffer_info info = arr.request();
    if (info.ndim != 2 || info.shape[0] != info.shape[1]) {
        throw std::runtime_error("Input must be a square 2D numpy array");
    }

    int n = static_cast<int>(info.shape[0]);
    auto buf = arr.unchecked<2>();

    // Allocate grid
    int **arr_2d = new int*[n];
    for (int i = 0; i < n; i++) {
        arr_2d[i] = new int[n];
        for (int j = 0; j < n; j++) {
            arr_2d[i][j] = buf(i,j);
        }
    }
    grid* g = init_grid(arr_2d, n);
    
    // Clean up temporary array (assuming init_grid makes its own copy)
    for (int i = 0; i < n; i++) {
        delete[] arr_2d[i];
    }
    delete[] arr_2d;

    return g;
}

// Wrap SolvedGrids -> list of numpy arrays
std::vector<py::array_t<int>> solve_dfs(py::array_t<int> arr) {
    grid* g = numpy_to_grid(arr);
    SolvedGrids* sols = solve_dfs_sudoku_c(g);

    std::vector<py::array_t<int>> out;
    for (int i = 0; i < sols->size; i++) {
        out.push_back(grid_to_numpy(sols->grids[i]));
    }

    free_SolvedGrids(sols);
    free_grid(g);
    return out;
}

std::vector<py::array_t<int>> solve_dlx(py::array_t<int> arr) {
    grid* g = numpy_to_grid(arr);
    SolvedGrids* sols = solve_dlx_naive_sudoku_c(g);

    std::vector<py::array_t<int>> out;
    for (int i = 0; i < sols->size; i++) {
        out.push_back(grid_to_numpy(sols->grids[i]));
    }

    free_SolvedGrids(sols);
    free_grid(g);
    return out;
}

PYBIND11_MODULE(CSudokuSolver, m) {
    m.def("solve_dfs", &solve_dfs, "Solve sudoku using DFS (input: numpy array)");
    m.def("solve_dlx", static_cast<std::vector<py::array_t<int>>(*)(py::array_t<int>)>(&solve_dlx), "Solve sudoku using DLX (input: numpy array)");
}
