//
// Created by Thomas Rambrant on 2025-05-08.
//
#pragma once

#include "SudokuTraits.hpp"

/**
 * @brief A reader interface for classes that can read a Sudoku board from some sort of source.
 */
class ISudokuWriter
{
    public:

        /// @brief Alias to shorten access to @ref SudokuTraits.
        using Traits = SudokuTraits;

        /**
         * @brief A method that writes a Sudoku board to destination.
         * @param board The board to print ou to the stream
         */
        virtual auto write( const Traits::Board& board) const -> void = 0;

        virtual ~ISudokuWriter() = default;
};