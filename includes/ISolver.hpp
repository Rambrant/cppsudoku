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
     * @brief A solver interface for classes that can solve a Sudoku board using some sort of algorithm.
     */
    class ISolver
    {
        public:

            /// @brief Alias to shorten access to @ref SudokuTraits.
            using Traits = SudokuTraits;

            /**
              * @brief Base class constructor that initiates the logger member to be used by the subclasses
              * @param logger The logger instance. Must be valid during the lifetime of the readers
              */
            explicit ISolver( const Logger& logger) : mLogger( logger) {};
            virtual ~ISolver() = default;

            /**
             * @brief Solves the given Sudoku board
             * @param board A SudokuTraits.Board. Usually a 9x9 grid with values 1-9 (zero representing no value).
             * @return A SudokuTraits.BoardResult
             */
            [[nodiscard]]
            virtual auto solve( Traits::Board& board) const -> Traits::BoardResult = 0;

        protected:

            /// @brief internal logger reference to be used by the subclasses to write log messages to
            const Logger& mLogger;
    };
}