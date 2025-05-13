//
//  BackTrackingSolver.hpp
//
//  Created by Thomas Rambrant on 2025-04-12.
//
#pragma once

#include <iostream>

#include "ISudokuSolver.hpp"

namespace com::rambrant::sudoku
{
    /**
     * @brief A Sudoku solver that uses a simple brute force backtracking algorithm
     */
    class BackTrackingSolver : public ISudokuSolver
    {
        public:

            /**
             * @brief Constructs the class with a Logger
             * @param logger The @ref Logger instance. Must be valid during the lifetime of the solver
             */
            explicit BackTrackingSolver( const Logger& logger);

            /**
             * @brief Solves the given Sudoku board using a brute force backtracking algorithm.
             * @param board A @ref SudokuTraits.Board.
             * @return A @ref SudokuTraits.BoardResult.
             */
            [[nodiscard]]
            auto solve( Traits::Board& board) const -> Traits::BoardResult override;

        private:

            mutable std::size_t mRecursions{ 0};

            bool solver( Traits::Board& board) const;

            static auto isValid( Traits::Board & board, int value, int rowPos, int columnPos ) -> bool;

            static auto rowConstraint( const Traits::Board & board, int value, int rowPos ) -> bool;
            static auto columnConstraint( const Traits::Board & board, int value, int columnPos ) -> bool;
            static auto boxConstraint( const Traits::Board & board, int value, int rowPos, int columnPos ) -> bool;

            static auto check( int value, const Traits::BoardArray & unitValues ) -> bool;
    };
}