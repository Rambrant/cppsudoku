//
//  BackTrackingSolver.hpp
//
//  Created by Thomas Rambrant on 2025-04-12.
//
#pragma once

#include "ISudokuSolver.hpp"

namespace com::rambrant::sudoku
{
    using Traits = SudokuTraits;

    /**
     * @brief A Sudoku solver that uses a simple brute force backtracking algorithm
     */
    class BackTrackingSolver : public ISudokuSolver
    {
        public:

            /**
             * @brief Constructs the class with a Logger
             * @param logger The @ref Logger instance. Must be valid during the lifetime of the solver
             */
            explicit BackTrackingSolver( const Logger& logger);

            /**
             * @brief Solves the given Sudoku board using a brute force backtracking algorithm.
             * @param board A @ref SudokuTraits.Board.
             * @return A @ref SudokuTraits.BoardResult.
             */
            [[nodiscard]]
            auto solve( Traits::Board& board) const -> Traits::BoardResult override;
    };
}