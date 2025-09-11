import React, { useState, useRef} from "react";
import "./SudokuGrid.css";
const backendUrl = import.meta.env.VITE_BACKEND_URL;
interface SudokuGridProps {
  size: number; // NxN grid
}

const SudokuGrid: React.FC<SudokuGridProps> = ({ size }) => {
  // Initialize NxN grid with empty strings
  const [grid, setGrid] = useState<string[][]>(
    Array.from({ length: size }, () => Array(size).fill(""))
  );

  const [solver,setSolver] = useState<string>("");

  const inputRefs = useRef<(HTMLInputElement | null)[][]>(
    Array.from({ length: size }, () => Array(size).fill(null))
  );

  const handleChangeSolver = (newSolver:string) =>{
    setSolver(newSolver);
    console.log("here",newSolver);
  }

  const handleChange = (row: number, col: number, value: string) => {
    if (/^[1-9]?$/.test(value)) { 
      const newGrid = grid.map((r, i) =>
        r.map((c, j) => (i === row && j === col ? value : c))
      );
      setGrid(newGrid);
    }
  };
  const handleKeyDown = (e: React.KeyboardEvent, row: number, col: number) => {
    let newRow = row;
    let newCol = col;

    switch (e.key) {
      case "ArrowUp":
        newRow = row > 0 ? row - 1 : row;
        break;
      case "ArrowDown":
        newRow = row < size - 1 ? row + 1 : row;
        break;
      case "ArrowLeft":
        newCol = col > 0 ? col - 1 : col;
        break;
      case "ArrowRight":
        newCol = col < size - 1 ? col + 1 : col;
        break;
      default:
        return;
    }

    e.preventDefault();
    inputRefs.current[newRow][newCol]?.focus();
  };


  const handleSubmit = async () => {
    if(solver === ""){
      alert("You did not choose the solver");
      return;
    }
    try {
      const response = await fetch(`${backendUrl}/api/submit_grid`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ 
          grid:grid,
          solver:solver 
          }),
        });

        const data = await response.json();

        if(data.status == 'success' && data.solution_count > 0){

          
          const solvedGrid: string[][] = data.solutions[0];
          
          setGrid(solvedGrid);
          console.log("This is response");
        }
        else if(data.status === 'success'){
          console.log("Solved");
        }
        else{
          alert("No solution");
        }

        console.log("Response from backend:", data);
    } catch (error) {
        console.error("Error sending grid:", error);
        }
    };


    return (
    <div>
      
      Sudoku Solver
      <div className="sudoku-grid" style={{ gridTemplateRows: `repeat(${size}, 1fr)` }}>
        {grid.map((row, i) => (
          <div key={i} className="sudoku-row" style={{ gridTemplateColumns: `repeat(${size}, 1fr)` }}>
            {row.map((cell, j) => (
              <input
                key={j}
                type="text"
                value={cell}
                onChange={(e) => handleChange(i, j, e.target.value)}
                onKeyDown={(e)=> handleKeyDown(e,i,j)}
                maxLength={1}
                ref={(el) => {
                  if (!inputRefs.current[i]) inputRefs.current[i] = [];
                  inputRefs.current[i][j] = el;
                }}
                className="sudoku-cell"
              />
            ))}
          </div>
        ))}
      </div>
       <button onClick={() => handleChangeSolver("DLX")}>
        DLX
      </button>
      <button onClick={() => handleChangeSolver("DFS")}>
        DFS
      </button>
      <button onClick={handleSubmit} style={{ marginTop: "20px" }}>
        Send Grid to Backend
      </button>
     
    </div>
  );
};

export default SudokuGrid;
