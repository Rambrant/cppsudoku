//
//  ISudokuReader.hpp
//
//  Created by Thomas Rambrant on 2025-04-18.
//
#pragma once

#include "SudokuTraits.hpp"

/**
 * @brief A reader interface for classes that can read a Sudoku board from some sort of source.
 */
class ISudokuReader
{
    public:

        /// @brief Alias to shorten access to @ref SudokuTraits.
        using Traits = SudokuTraits;

        /**
         * @brief A method that reads a Sudoku board from a source.
         * @return An initialized @ref SudokuTraits.Board. Usually a 9x9 grid with values 1-9 (zero representing no value).
         */
        [[nodiscard]]
        virtual auto read() const -> Traits::Board = 0;

        virtual ~ISudokuReader() = default;
};
