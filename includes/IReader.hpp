//
//  ISudokuReader.hpp
//
//  Created by Thomas Rambrant on 2025-04-18.
//
#pragma once

#include "SudokuTraits.hpp"

namespace com::rambrant::sudoku
{
    class Logger;

    /**
     * @brief A reader interface for classes that can read a Sudoku board from some sort of source.
     */
    class IReader
    {
        public:

            /**
             * @brief Base class constructor that initiates the logger member to be used by the subclasses
             * @param logger The logger instance. Must be valid during the lifetime of the readers
             */
            explicit IReader( const Logger& logger) : mLogger( logger) {};
            virtual  ~IReader() = default;

            /// @brief Alias to shorten access to @ref SudokuTraits.
            using Traits = SudokuTraits;

            /**
             * @brief A method that reads a Sudoku board from a source.
             * @return An initialized @ref SudokuTraits.Board. Usually a 9x9 grid with values 1-9 (zero representing no value).
             */
            [[nodiscard]]
            virtual auto read() const -> Traits::Board = 0;

        protected:

            /// @brief internal logger reference to be used by the subclasses to write log messages to
            const Logger& mLogger;
    };
}