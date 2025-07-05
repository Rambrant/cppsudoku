//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <array>
#include <tuple>

#include "ConstSqrt.hpp"
#include "RangeArray.hpp"

namespace com::rambrant::sudoku
{
    /**
     * @brief Basic constants and type definitions for a Sudoku board.
     */
    struct SudokuTraits
    {
        /// @brief Size of the Sudoku board. Usually a 9x9 grid
        static constexpr int  BOARD_SIZE = 9;
        static_assert( BOARD_SIZE >= 0, "BOARD_SIZE must be positive integer");

        /// @brief Size of a box on the board
        static constexpr int BOX_SIZE = ConstSqrt( BOARD_SIZE);
        static_assert( BOX_SIZE * BOX_SIZE == BOARD_SIZE, "BOARD_SIZE must be a perfect square");

        /// @brief The maximum value for a square
        static constexpr int  MAX_VALUE = BOARD_SIZE;

        /// @brief Denotes the presence of no values at all
        static constexpr int  NO_VALUE = 0;

        /// @brief Range from 0 to the @ref BOARD_SIZE
        static constexpr auto INDEX_RANGE = RangeArray<BOARD_SIZE>(0, 1);

        /// @brief Range of possible values in each board cell, 1 to the MAX_VALUE
        static constexpr auto VALUE_RANGE = RangeArray<MAX_VALUE>(1, 1);

        /// @brief The board single digit '0' up to the @ref BOARD_SIZE in hexadecimal if needed.
        using Digit = char;

        /// @brief The internal value for each board cell.
        using Value = int;

        /// @brief Result from a solver. Success or not and the number of recursions
        using BoardResult = std::tuple< bool, std::size_t>;

        /// @brief Board row of size @ref BOARD_SIZE
        using BoardArray = std::array< Value, BOARD_SIZE>;

        /// @brief 2D grid of @ref BOARD_SIZE number of @ref BoardArray's
        using Board = std::array< BoardArray, BOARD_SIZE>;
    };

    /**
     * @brief A trait for checking iterators for random_access
     * @tparam Container The container to test for random_access iterator type
     */
    template <typename Container>
    constexpr bool is_random_access_container = std::is_base_of_v< std::random_access_iterator_tag,
        typename std::iterator_traits< typename Container::iterator>::iterator_category>;}