//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <flat_map>
#include <flat_set>
#include <vector>
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


            /// @brief The solver key used to select this solver from the command line.
            static constexpr std::string_view pluginKey = "constraint";

            /**
             * @brief Constructs the class with a Logger
             * @param logger The @ref Logger instance. Must be valid during the lifetime of the solver
             */
            explicit ConstraintPropagationSolver( const Logger& logger);
            ~ConstraintPropagationSolver() override = default;

            /**
             * @brief Solves the given Sudoku board using a constraint propagation algorithm.
             * @param board A @ref SudokuTraits::Board.
             * @param cancelFlag A reference to an atomic flag used to signal the premature exit of thread
             * @return A @ref SudokuTraits::BoardResult.
             */
            [[nodiscard]]
            auto solve( Traits::Board& board, std::atomic<bool>& cancelFlag ) const -> Traits::BoardResult override;

            /**
             * @brief Returns @c "constraint" — the command-line key for this solver.
             * @see ISolver::name()
             */
            [[nodiscard]]
            auto name() const noexcept -> std::string_view override
            {
                return pluginKey;
            }
    };

    namespace detail
    {
        //
        // The control structure defining the squares, units and peers for the Sudoku board
        //
        using Square        = std::pair<int, int>;                          // Row and column index
        using Squares       = std::flat_set<Square>;
        using SquareValues  = std::flat_map<Square, std::vector<Traits::Value>>;
        using Units         = std::flat_map<Square, std::vector<Squares>>;
        using Peers         = std::flat_map<Square, Squares>;

        /**
         * @brief Internal struct used to hold the structure of peers and units associated with a board square
         */
        struct BoardStructure
        {
            Squares mSquares{}; /*!< All the square coordinates */
            Units   mUnits{};   /*!< All the units associated with square */
            Peers   mPeers{};   /*!< All the peers associated with square */

            BoardStructure();
        };

        const BoardStructure gBoardsStructure;

        //
        // Helper functions
        //
        auto eliminate( SquareValues& allValues, const Square& square, Traits::Value value ) -> bool;
        auto assign( SquareValues& allValues, const Square& square, Traits::Value value ) -> bool;
        auto search( SquareValues allValues, int& recursions, std::atomic<bool>& cancelFlag) -> SquareValues;
        auto parseGrid( const Traits::Board & board) -> SquareValues;
    }
}