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
    class ISudokuReader;
    class ISudokuWriter;
    class ISudokuSolver;

    /**
     * @brief A Sudoku solver that uses a constraint propagation algorithm. Modeled after the idea from Peter Norvig, @see https://norvig.com/sudoku.html
     */
    class SudokuBoard
    {
        public:

            /// @brief Alias to shorten access to @ref SudokuTraits.
            using Traits = SudokuTraits;

            /// @brief A list of references to @ref ISudokuSolver references
            using SolverList = std::vector<std::reference_wrapper<ISudokuSolver>>;

            /**
             * @brief Constructs the board and initializes the reader.
             * @param reader An @ref ISudokuReader. The reader must exist during the lifetime of the SudokuBoard
             * @param writer A @ref ISudokuWriter. The writer must exist during the lifetime of the SudokuBoard
             * @param solvers A list of @ref ISudokuSolver. The solvers must exist during the lifetime of the SudokuBoard
             * @param logger A @ref Logger. The logger must exist during the lifetime of the board
             */
            SudokuBoard( const ISudokuReader& reader, const ISudokuWriter& writer, SolverList solvers, const Logger& logger);

            /**
             * @brief Reads the board from the @ref ISudokuReader given in the constructor
             */
            auto read() const -> void;

            /**
             * @brief Writes the board using the @ref ISudokuWriter given in the constructor
             */
            auto write() const -> void;

            /**
             * @brief Solves the Sudoku board using the solvers given in the constructor, The solvers are tried in the given order
             * @return A SudokuTraits.BoardResult
             */
            auto solve() const -> Traits::BoardResult;

        private:

            mutable Traits::Board  mBoard;
            const   Logger&        mLogger;
            const   ISudokuReader& mReader;
            const   ISudokuWriter& mWriter;
            const   SolverList     mSolvers;
    };
}