//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include "SudokuTraits.hpp"

namespace com::rambrant::sudoku
{
    /**
     * @brief Checks whether a Sudoku board is valid according to the standard constraints.
     *
     * A board is valid if no row, column or box contains a duplicate non-zero value.
     * Empty cells (zero) are ignored. The board does not need to be fully filled.
     *
     * This is a pure function with no side effects — it can be called freely from
     * any layer: @ref SudokuBoard::read(), @ref SolveCommand, tests, etc.
     *
     * @param board The board to validate.
     * @return true if the board satisfies all Sudoku constraints, false otherwise.
     */
    [[nodiscard]]
    bool isValidBoard( const SudokuTraits::Board& board);
}