//
// Created by Thomas Rambrant on 2025-05-09.
//

#pragma once

#include "ISudokuWriter.hpp"

namespace com::rambrant::sudoku
{
    /**
     * @brief A board writer that prints out a pretty formatted output to the stream
     */
    class SudokuBlockWriter : public ISudokuWriter
    {
        public:

            /**
             * @brief Constructs the class that writes the board in a "block". A one row per line
             * @param os The existing outstream to write to.
             * @param logger The @ref Logger instance. Must be valid during the lifetime of the writer
             */
            SudokuBlockWriter( std::ostream& os, const Logger& logger);

            /**
             * @brief Writes the given board to the stream
             * @param board The board to write out.
             */
            auto write( const Traits::Board& board) const -> void override;

        private:

            std::ostream& mStream;
    };
}
