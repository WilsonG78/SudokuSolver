import React, { useState } from "react";
import "./SudokuGrid.css";

interface SudokuGridProps {
  size: number; // NxN grid
}

const SudokuGrid: React.FC<SudokuGridProps> = ({ size }) => {
  // Initialize NxN grid with empty strings
  const [grid, setGrid] = useState<string[][]>(
    Array.from({ length: size }, () => Array(size).fill(""))
  );

  const handleChange = (row: number, col: number, value: string) => {
    if (/^[1-9]?$/.test(value)) { // allow only 1–9 or empty
      const newGrid = grid.map((r, i) =>
        r.map((c, j) => (i === row && j === col ? value : c))
      );
      setGrid(newGrid);
    }
  };

  const handleSubmit = async () => {
    try {
      const response = await fetch("/api/submit_grid", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ grid }), // Send as JSON
        });

        const data = await response.json();
        console.log("Response from backend:", data);
        alert("Grid submitted successfully!");
    } catch (error) {
        console.error("Error sending grid:", error);
        }
    };

    return (
    <div>
      <div className="sudoku-grid" style={{ gridTemplateRows: `repeat(${size}, 1fr)` }}>
        {grid.map((row, i) => (
          <div key={i} className="sudoku-row" style={{ gridTemplateColumns: `repeat(${size}, 1fr)` }}>
            {row.map((cell, j) => (
              <input
                key={j}
                type="text"
                value={cell}
                onChange={(e) => handleChange(i, j, e.target.value)}
                maxLength={1}
                className="sudoku-cell"
              />
            ))}
          </div>
        ))}
      </div>
      <button onClick={handleSubmit} style={{ marginTop: "20px" }}>
        Send Grid to Backend
      </button>
    </div>
  );
};

export default SudokuGrid;
