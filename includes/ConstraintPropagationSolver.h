//
// Created by Thomas Rambrant on 2025-04-29.
//
#pragma once

#include <ISudokuSolver.hpp>
#include <map>
#include <set>
#include <utility>

class ConstraintPropagationSolver : public ISudokuSolver
{
public:

    using Traits   = SudokuTraits;
    using CellKey  = std::pair< int, int>;
    using CellList = std::set< CellKey>;
    using ValueMap = std::map< CellKey, std::vector<int>>;
    using CellMap  = std::map< CellKey, CellList>;

    ConstraintPropagationSolver();
    ~ConstraintPropagationSolver() override = default;

    auto solve( Traits::Board& board) const -> Traits::BoardResult override;

private:

    mutable std::size_t mRecursions{};

    mutable CellList mCellKeys{}; // All coordinates of the board cells
    mutable ValueMap mValues{};   // The current possible values for a cell
    mutable CellMap  mUnits{};    // All the cells in the same row, column and subsection as the cell
    mutable CellMap  mPeers{};    // Same as mUnits but without the actual cell

    static auto createAllKeys( const Traits::BoardArray& rowRange, const Traits::BoardArray& colRange) -> CellList;
    static auto createSubsectionKeys( const CellKey& key) -> CellList;

    auto solver( Traits::Board& board) const -> bool;
};
