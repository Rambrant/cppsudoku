//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <tuple>
#include <tuple>

#include "ISolver.hpp"

namespace com::rambrant::sudoku
{
    using Traits = SudokuTraits;

    /**
     * @brief A Sudoku solver that uses a constraint propagation algorithm. Modeled after the idea from Peter Norvig, @see https://norvig.com/sudoku.html
     */
    class ConstraintPropagationSolver : public ISolver
    {
        public:

            /**
             * @brief Constructs the class with a Logger
             * @param logger The @ref Logger instance. Must be valid during the lifetime of the solver
             */
            explicit ConstraintPropagationSolver( const Logger& logger);
            ~ConstraintPropagationSolver() override = default;

            /**
             * @brief Solves the given Sudoku board using a constraint propagation algorithm.
             * @param board A @ref SudokuTraits.Board.
             * @param cancelFlag A reference to an atomic flag used to signal the premature exit of thread
             * @return A @ref SudokuTraits.BoardResult.
             */
            [[nodiscard]]
            auto solve( Traits::Board board, std::atomic<bool>& cancelFlag ) const -> Traits::BoardResult override;
    };
}