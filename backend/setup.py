from setuptools import setup, Extension
from pybind11.setup_helpers import Pybind11Extension, build_ext
import pybind11
import os, sys


sources_cpp = ["sudoku_solver/sudoku_bindings.cpp"]

sources_c = [        
    "sudoku_solver/c_solver.c",
    "sudoku_solver/data_structures_C/grid.c",
    "sudoku_solver/data_structures_C/array_2d.c",
    "sudoku_solver/data_structures_C/set.c",
    "sudoku_solver/data_structures_C/DLX.c",
    "sudoku_solver/data_structures_C/ListOfLists.c",
    "sudoku_solver/data_structures_C/read_data.c",
]

# Define the extension module
ext_modules = [
    Pybind11Extension(
        "CSudokuSolver",
        sources_cpp + sources_c,
        
        include_dirs=[
            ".",
            "data_structures_C",
            pybind11.get_include(),
        ],
        language="c++",
        extra_compile_args=["/EHsc", "/MD"] if sys.platform == "win32" else [],
        cxx_std=11,
    ),
]

setup(
    name="sudoku-solver",
    version="0.1.0",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    python_requires=">=3.8",
    install_requires=[
        "numpy",
        "pybind11",
    ],
)
