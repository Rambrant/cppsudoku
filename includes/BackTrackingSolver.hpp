//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include "ISolver.hpp"

namespace com::rambrant::sudoku
{
    using Traits = SudokuTraits;

    /**
     * @brief A Sudoku solver that uses a simple brute force backtracking algorithm
     */
    class BackTrackingSolver : public ISolver
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