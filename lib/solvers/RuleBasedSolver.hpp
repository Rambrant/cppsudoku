//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <array>
#include <expected>
#include <string_view>
#include <vector>

#include "ISolver.hpp"

namespace com::rambrant::sudoku
{
    using Traits = SudokuTraits;

    /**
     * @brief A Sudoku solver that applies human-like logical deduction rules.
     *
     * Rules applied in escalating order of complexity:
     *   1. @b Naked Single       — a cell with exactly one candidate; eliminate from unit peers.
     *   2. @b Hidden Single      — a value that fits only one cell within a row, column, or box.
     *   3. @b Naked Pair         — two cells in a unit whose union of candidates is exactly two
     *                              values; those values are eliminated from all other unit cells.
     *   4. @b Hidden Pair        — two values confined to the same two cells in a unit; all other
     *                              candidates are removed from those two cells.
     *   5. @b Pointing Pair      — all occurrences of a value inside a box lie in one row or
     *                              column; eliminated from the rest of that row or column.
     *   6. @b Box-Line Reduction — all occurrences of a value inside a row or column lie in one
     *                              box; eliminated from the rest of that box.
     *
     * If no rule makes progress the solver returns @c solved = false without further modifying
     * the board.  Use @c constraint or @c backtracking for guaranteed completion on puzzles that
     * require trial-and-error.
     */
    class RuleBasedSolver final : public ISolver
    {
        public:

            /// @brief The solver key used to select this solver from the command line.
            static constexpr std::string_view pluginKey = "rules";

            /**
             * @brief Constructs the class with a Logger.
             * @param logger The @ref Logger instance. Must be valid during the lifetime of the solver.
             */
            explicit RuleBasedSolver( const Logger& logger);

            /**
             * @brief Solves the board by repeatedly applying logical deduction rules.
             *
             * Terminates early on @p cancelFlag.  Returns @c solved = false when a contradiction
             * is detected or when no rule yields further progress (board requires guessing).
             *
             * @param board     The board to solve in-place on success.
             * @param cancelFlag Set to @c true by the solver on success to terminate racing solvers.
             * @return @ref SudokuTraits::BoardResult with @c solved, pass count, and result board.
             */
            [[nodiscard]]
            auto solve( Traits::Board& board, std::atomic<bool>& cancelFlag) const -> Traits::BoardResult override;

            /**
             * @brief Returns @c "rules" — the command-line key for this solver.
             * @see ISolver::name()
             */
            [[nodiscard]]
            auto name() const noexcept -> std::string_view override { return pluginKey; }
    };

    namespace detail
    {
        /// @brief Candidate set for a single board cell (remaining possible values).
        using CandidateSet  = std::vector<Traits::Value>;

        /// @brief 9×9 grid of candidate sets, indexed as @c grid[row][col].
        using CandidateGrid = std::array<std::array<CandidateSet, Traits::BOARD_SIZE>, Traits::BOARD_SIZE>;

        /**
         * @brief Initialises the candidate grid from a board.
         *
         * Empty cells receive all nine candidates; given cells are fixed to their value and
         * that value is eliminated from every peer immediately.
         *
         * @return The initialised grid, or an error string if the board contains a contradiction.
         */
        [[nodiscard]]
        auto initCandidates( const Traits::Board& board) -> std::expected<CandidateGrid, std::string>;

        /// @brief Returns @c true when every cell has exactly one candidate (board is solved).
        [[nodiscard]]
        auto isSolved( const CandidateGrid& grid) -> bool;

        /// @brief Returns @c true when any cell has no candidates remaining.
        [[nodiscard]]
        auto isContradicted( const CandidateGrid& grid) -> bool;

        /// @brief Converts a solved grid back to a @ref SudokuTraits::Board.
        [[nodiscard]]
        auto toBoard( const CandidateGrid& grid) -> Traits::Board;

        //
        // Rules — each returns true when at least one candidate was eliminated.
        //

        /// @brief Rule 1 — Naked Single.
        [[nodiscard]] auto applyNakedSingles     ( CandidateGrid& grid) -> bool;
        /// @brief Rule 2 — Hidden Single.
        [[nodiscard]] auto applyHiddenSingles    ( CandidateGrid& grid) -> bool;
        /// @brief Rule 3 — Naked Pair.
        [[nodiscard]] auto applyNakedPairs       ( CandidateGrid& grid) -> bool;
        /// @brief Rule 4 — Hidden Pair.
        [[nodiscard]] auto applyHiddenPairs      ( CandidateGrid& grid) -> bool;
        /// @brief Rule 5 — Pointing Pair/Triple.
        [[nodiscard]] auto applyPointingPairs    ( CandidateGrid& grid) -> bool;
        /// @brief Rule 6 — Box-Line Reduction.
        [[nodiscard]] auto applyBoxLineReduction ( CandidateGrid& grid) -> bool;
    }
}