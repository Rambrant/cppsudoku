

//
// Created by Thomas Rambrant on 2025-05-09.
//

#pragma once

#include "ISudokuWriter.hpp"

namespace com::rambrant::sudoku
{
    class Logger;

    /**
     * @brief A board writer that prints out the board as a single line with whitespace separated values
     */
    class SudokuLineWriter : public ISudokuWriter
    {
        public:

            /**
             * @brief Constructs the class that writes the board in a single line
             * @param os The existing outstream to write to.
             * @param logger The @ref Logger instance. Must be valid during the lifetime of the writer
             */
            SudokuLineWriter( std::ostream& os, const Logger& logger);

            /**
             * @brief Writes the given board to the stream
             * @param board The board to write out.
             */
            auto write( const Traits::Board& board) const -> void override;

        private:

            std::ostream& mStream;
    };
}