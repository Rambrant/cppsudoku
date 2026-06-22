//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <memory>
#include <vector>

#include "SudokuTraits.hpp"


namespace com::rambrant::sudoku
{
    class Logger;
    class IReader;
    class IWriter;
    class ISolver;

    /**
     *  @brief Manages reading, solving, and writing a Sudoku board using injected strategies.
     */
    class SudokuBoard
    {
        public:

            /// @brief Alias to shorten access to @ref SudokuTraits.
            using Traits = SudokuTraits;

            /// @brief A list of references to @ref ISolver references
            using SolverList = std::vector<std::unique_ptr<ISolver>>;

            /**
             * @brief Constructs the board and initializes the reader.
             * @param reader An @ref IReader. The reader must exist during the lifetime of the SudokuBoard
             * @param writer A @ref IWriter. The writer must exist during the lifetime of the SudokuBoard
             * @param solvers A list of @ref ISolver. The solvers must exist during the lifetime of the SudokuBoard
             * @param logger A @ref Logger. The logger must exist during the lifetime of the board
             */
            SudokuBoard( const IReader& reader, const IWriter& writer, SolverList solvers, const Logger& logger);

            /**
             * @brief Reads the board from the @ref IReader given in the constructor
             */
            auto read() const -> void;

            /**
             * @brief Writes the board using the @ref IWriter given in the constructor
             */
            auto write() const -> void;

            /**
             * @brief Solves the Sudoku board using the solvers given in the constructor, The solvers are tried in the given order
             * @return A boolean indicating success or failure
             * @post If @c true is returned, no cell on the board contains @ref SudokuTraits::NO_VALUE.
             */
            auto solve() const -> bool
                post( result: ! result || isSolved());

        private:

            /**
             * @brief Returns @c true if every cell on the board holds a non-zero value.
             * @note Used internally to verify the postcondition of @ref solve().
             */
            [[nodiscard]] bool isSolved() const;

            mutable Traits::Board  mBoard;
            const   Logger&        mLogger;
            const   IReader&       mReader;
            const   IWriter&       mWriter;
            const   SolverList     mSolvers;
    };
}