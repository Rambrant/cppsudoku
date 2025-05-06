//
// Created by Thomas Rambrant on 2025-04-29.
//
#pragma once

#include <map>
#include <set>
#include <utility>
#include <optional>

#include "ISudokuSolver.hpp"

/**
 * @brief A Sudoku solver that uses a constraint propagation algorithm. Modeled after the idea from Peter Norvig, @see https://norvig.com/sudoku.html
 */
class ConstraintPropagationSolver : public ISudokuSolver
{
    public:

        /// @brief The coordinates to a cell (square) on the Sudoku board.
        using CellKey  = std::pair< int, int>;

        /// @brief A list of unique coordinates
        using CellList = std::set< CellKey>;

        /// @brief A Map for all possible values in a cell with a @ref CellKey as key
        using ValueMap = std::map< CellKey, std::vector<int>>;

        /// @brief A Map of @ref CellKey's containing all possible units for a cell, (All cells in  the same row, column and subsection).
        using UnitMap  = std::map< CellKey, std::vector<CellList>>;

        /// @brief A Map of @ref CellKey's containing all peers for a cell, (UnitMap flattened and without the actual cell).
        using PeerMap  = std::map< CellKey, CellList>;

        ConstraintPropagationSolver();
        ~ConstraintPropagationSolver() override = default;

        /**
         * @brief Solves the given Sudoku board using a constraint propagation algorithm.
         * @param board A @ref SudokuTraits.Board.
         * @return A @ref SudokuTraits.BoardResult.
         */
        [[nodiscard]]
        auto solve( Traits::Board& board) const -> Traits::BoardResult override;

    private:

        mutable std::size_t mRecursions{};

        CellList mCellKeys{}; // All coordinates of the board cells
        UnitMap  mUnits{};    // A list of all units, the cells in the row, column and subsection the cell belongs to
        PeerMap  mPeers{};    // All the peers to the cell. mUnits flattened and without the actual cell

        auto parseGrid( const Traits::Board& board) const -> ValueMap;
        auto assign( ValueMap& valuesMap, const CellKey& key, int value) const -> bool;
        auto eliminate( ValueMap& valuesMap, const CellKey& key, int value) const -> bool;
        auto search( ValueMap valuesMap) const -> ValueMap;
};
