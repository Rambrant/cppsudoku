//
//  SudokoBoard.hpp
//
//  Created by Thomas Rambrant on 2025-04-07.
//
#pragma once

#include <iostream>

#include "SudokuTraits.hpp"

class ISudokuReader;
class ISudokuSolver;

/**
 * @brief A Sudoku solver that uses a constraint propagation algorithm. Modeled after the idea from Peter Norvig, @see https://norvig.com/sudoku.html
 */
class SudokuBoard
{
    public:
    
        /// @brief Alias to shorten access to @ref SudokuTraits.
        using Traits = SudokuTraits;
        
        /**
         * @brief Constructs the board and initializes the reader.
         * @param reader An existing object of @ref ISudokuReader type.
         */
        explicit SudokuBoard( const ISudokuReader& reader);
        
        /**
         * @brief Solves the Sudoku board
         * @param solver An existing object of @ref ISudokuSolver type.
         * @return A SudokuTraits.BoardResult
         */
        auto solve( const ISudokuSolver& solver) -> Traits::BoardResult;
            
        /**
         * @brief Prints a formatted board to an output stream
         * @param os An output stream.
         * @param board A SudokuBoard.
         * @return A SudokuTraits.BoardResult
         */
        friend std::ostream& operator<<(std::ostream& os, const SudokuBoard& board);
    
    private:
    
        Traits::Board mBoard;
};
