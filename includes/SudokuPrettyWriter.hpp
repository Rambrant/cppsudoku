//
// Created by Thomas Rambrant on 2025-05-08.
//
#pragma once

#include <ostream>

#include "ISudokuWriter.hpp"

/**
 * @brief A board writer that prints out a pretty formatted output to the stream
 */
class SudokuPrettyWriter : public ISudokuWriter
{
    public:

        /**
         * @brief Constructs the class and writes the board.
         * @param os The existing in stream to write to.
         */
        explicit SudokuPrettyWriter( std::ostream& os);

        /**
         * @brief Writes the given board to the stream
         * @param board The board to write out.
         */
        auto write( const Traits::Board& board) const -> void override;

    private:

        std::ostream& mStream;
};