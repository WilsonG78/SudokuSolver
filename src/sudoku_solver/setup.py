from setuptools import setup, Extension
from pybind11.setup_helpers import Pybind11Extension, build_ext
import pybind11
import os, sys

# Define the extension module
ext_modules = [
    Pybind11Extension(
        "CSudokuSolver",
        [
            "sudoku_bindings.cpp",
            "c_solver.c",
            "data_structures_C/grid.c",
            "data_structures_C/array_2d.c",
            "data_structures_C/set.c",
            "data_structures_C/DLX.c",
            "data_structures_C/ListOfLists.c",
            "data_structures_C/read_data.c",
        ],
        include_dirs=[
            ".",
            "data_structures_C",
            pybind11.get_include(),
        ],
        language="c++",
        extra_compile_args=["/EHsc", "/MT"] if sys.platform == "win32" else [],
        cxx_std=11,
    ),
]

setup(
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
)
