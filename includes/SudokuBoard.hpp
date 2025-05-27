//
//  Created by Thomas Rambrant on 2025-04-07.
//
#pragma once

#include <functional>
#include <vector>

#include "SudokuTraits.hpp"


namespace com::rambrant::sudoku
{
    class Logger;
    class IReader;
    class IWriter;
    class ISolver;

    /**
     * @brief A Sudoku solver that uses a constraint propagation algorithm. Modeled after the idea from Peter Norvig, @see https://norvig.com/sudoku.html
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
             * @return A SudokuTraits.BoardResult
             */
            auto solve() const -> bool;

        private:

            mutable Traits::Board  mBoard;
            const   Logger&        mLogger;
            const   IReader& mReader;
            const   IWriter& mWriter;
            const   SolverList     mSolvers;
    };
}