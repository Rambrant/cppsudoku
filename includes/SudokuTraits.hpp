//
//  Created by Thomas Rambrant on 2025-04-11.
//
#pragma once

#include <array>
#include <tuple>

#include "SudokuUtil.hpp"

namespace com::rambrant::sudoku
{
    /**
     * @brief Basic constants and type definitions for a Sudoku board.
     */
    struct SudokuTraits
    {
        /// @brief Size of the Sudoku board. Usually a 9x9 grid
        static constexpr int  BOARD_SIZE  = 9;

        /// @brief Size of a box on the board
        static constexpr int  BOX_SIZE    = 3;

        /// @brief The maximum value for a square
        static constexpr int  MAX_VALUE   = BOARD_SIZE;

        /// @brief Denotes the presence of no values at all
        static constexpr int  NO_VALUE    = 0;

        /// @brief Range from 0 to the @ref BOARD_SIZE
        static constexpr auto INDEX_RANGE = RangeArray<BOARD_SIZE>(0, 1);

        /// @brief Range of possible values in each board cell, 1 to the MAX_VALUE
        static constexpr auto VALUE_RANGE = RangeArray<MAX_VALUE>(1, 1);

        /// @brief Result from a solver. Success or not and the number of recursions
        using BoardResult                 = std::tuple< bool, std::size_t>;

        /// @brief Board row of size @ref BOARD_SIZE
        using BoardArray                  = std::array< int, BOARD_SIZE>;

        /// @brief 2D grid of @ref BOARD_SIZE number of @ref BoardArray's
        using Board                       = std::array< BoardArray, BOARD_SIZE>;
    };
}