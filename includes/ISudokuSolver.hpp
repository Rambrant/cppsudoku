//
//  ISudokuSolver.hpp
//
//  Created by Thomas Rambrant on 2025-04-07.
//
#pragma once

#include "SudokuTraits.hpp"

namespace com::rambrant::sudoku
{
    /**
     * @brief A solver interface for classes that can solve a Sudoku board using some sort of algorithm.
     */
    class ISudokuSolver
    {
        public:

            /// @brief Alias to shorten access to @ref SudokuTraits.
            using Traits = SudokuTraits;

            /**
             * @brief Solves the given Sudoku board
             * @param board A SudokuTraits.Board. Usually a 9x9 grid with values 1-9 (zero representing no value).
             * @return A SudokuTraits.BoardResult
             */
            [[nodiscard]]
            virtual auto solve( Traits::Board& board) const -> Traits::BoardResult = 0;

            virtual ~ISudokuSolver() = default;
    };
}