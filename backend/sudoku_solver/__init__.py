"""Sudoku Solver Package"""

# Try to import from different possible locations
try:
    # Try importing from the package directory
    from .CSudokuSolver import solve_dfs as solve_dfs_c, solve_dlx as solve_dlx_c
except ImportError:
    try:
        # Try importing from parent directory
        from CSudokuSolver import solve_dfs as solve_dfs_c, solve_dlx as solve_dlx_c
    except ImportError:
        # Fallback: add parent directory to path
        import sys
        import os
        sys.path.insert(0, os.path.dirname(os.path.dirname(__file__)))
        from CSudokuSolver import solve_dfs as solve_dfs_c, solve_dlx as solve_dlx_c

from .grid import Grid

__all__ = ['Grid', 'solve_dfs_c', 'solve_dlx_c']