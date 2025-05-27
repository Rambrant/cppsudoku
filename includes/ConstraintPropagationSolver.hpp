//
// Created by Thomas Rambrant on 2025-04-29.
//
#pragma once

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
             * @return A @ref SudokuTraits.BoardResult.
             */
            [[nodiscard]]
            auto solve( Traits::Board& board) const -> Traits::BoardResult override;
    };
}