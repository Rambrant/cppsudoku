//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include "SudokuTraits.hpp"

namespace com::rambrant::sudoku
{
    class Logger;

    /**
     * @brief A reader interface for classes that can read a Sudoku board from some sort of source.
     */
    class IWriter
    {
        public:

            /// @brief Alias to shorten access to @ref SudokuTraits.
            using Traits = SudokuTraits;

            /**
             * @brief Base class constructor that initiates the logger member to be used by the subclasses
             * @param logger The logger instance. Must be valid during the lifetime of the writers
             */
            explicit IWriter( const Logger& logger) : mLogger( logger) {};
            virtual ~IWriter() = default;

            /**
             * @brief A method that writes a Sudoku board to destination.
             * @param board The board to print ou to the stream
             */
            virtual auto write( const Traits::Board& board) const -> void = 0;


        protected:

            /// @brief internal logger reference to be used by the subclasses to write log messages to
            const Logger& mLogger;
    };
}