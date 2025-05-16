//
// Created by Thomas Rambrant on 2025-04-29.
//
#pragma once

#include <map>
#include <set>
#include <vector>
#include <utility>

#include "ISudokuSolver.hpp"

namespace com::rambrant::sudoku
{
    /**
     * @brief A Sudoku solver that uses a constraint propagation algorithm. Modeled after the idea from Peter Norvig, @see https://norvig.com/sudoku.html
     */
    class ConstraintPropagationSolver : public ISudokuSolver
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

        private:

            using Square       = std::pair< int, int>;                      // Coordinate of a single square on the Sudoku board.
            using Squares      = std::set< Square>;                         // A set of unique square positions.
            using SquareValues = std::map< Square, std::vector<int>>;       // Map from each square to its current list of possible values.
            using Units        = std::map< Square, std::vector<Squares>>;   // Map from each square to its associated units (row, column, box).
            using Peers        = std::map< Square, Squares>;                // Map from each square to its peers (other squares in its units, excluding itself).

            Squares mSquares{};  // All squares on the board
            Units   mUnits{};    // Units (row, column, section) each square belongs to.
            Peers   mPeers{};    // Peers of each square (flattened units minus the square).

            auto parseGrid( const Traits::Board& board) const -> SquareValues;
            auto assign( SquareValues& allValues, const Square& square, int value) const -> bool;
            auto eliminate( SquareValues& allValues, const Square& square, int value) const -> bool;
            auto search( SquareValues allValues, int & recursions ) const -> SquareValues;
    };
}