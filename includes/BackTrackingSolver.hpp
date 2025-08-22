//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <tuple>

#include "ISolver.hpp"

namespace com::rambrant::sudoku
{
    using Traits = SudokuTraits;

    /**
     * @brief A Sudoku solver that uses a simple brute force backtracking algorithm
     */
    class BackTrackingSolver final : public ISolver
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
             * @param cancelFlag A reference to an atomic flag used to signal the premature exit of thread
             * @return A @ref SudokuTraits.BoardResult.
             */
            [[nodiscard]]
            auto solve( Traits::Board& board, std::atomic<bool>& cancelFlag ) const -> Traits::BoardResult override;
    };

    namespace detail
    {
        //
        // Helper functions
        //
        auto checkValue( Traits::Value value, const Traits::BoardArray & unitValues) -> bool;
        auto rowConstraint( const Traits::Board & board, Traits::Value value, int rowPos) -> bool;
        auto columnConstraint( const Traits::Board & board, Traits::Value value, int columnPos) -> bool;
        auto boxConstraint( const Traits::Board & board, Traits::Value value, int rowPos, int columnPos) -> bool;
        auto setValid( Traits::Board& board, Traits::Value value, int rowPos, int columnPos) -> bool;
        auto search( Traits::Board & board, int& recursions, std::atomic<bool>& cancelFlag) -> bool;
    }
}