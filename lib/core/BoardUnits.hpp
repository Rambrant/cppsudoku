//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include "SudokuTraits.hpp"

namespace com::rambrant::sudoku
{
    using Traits = SudokuTraits;

    /**
     * @brief Extracts a column from the board as a flat array.
     *
     * @param board  The board to extract from.
     * @param col    0-based column index.
     * @return A @ref SudokuTraits::BoardArray containing the column values.
     * @pre @p col must be in the range [0, @ref SudokuTraits::BOARD_SIZE).
     */
    [[nodiscard]]
    auto extractColumn( const Traits::Board& board, int col) -> Traits::BoardArray
        pre( col >= 0 && col < Traits::BOARD_SIZE);

    /**
     * @brief Extracts the 3x3 box containing (row, col) as a flat array.
     *
     * @param board  The board to extract from.
     * @param row    0-based row index of any cell inside the target box.
     * @param col    0-based column index of any cell inside the target box.
     * @return A @ref SudokuTraits::BoardArray containing the box values in row-major order.
     * @pre @p row must be in the range [0, @ref SudokuTraits::BOARD_SIZE).
     * @pre @p col must be in the range [0, @ref SudokuTraits::BOARD_SIZE).
     */
    [[nodiscard]]
    auto extractBox( const Traits::Board& board, int row, int col) -> Traits::BoardArray
        pre( row >= 0 && row < Traits::BOARD_SIZE)
        pre( col >= 0 && col < Traits::BOARD_SIZE);
}
