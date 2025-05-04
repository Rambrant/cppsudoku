//
// Created by Thomas Rambrant on 2025-04-29.
//
#pragma once

#include <map>
#include <set>
#include <utility>
#include <optional>


#include "ISudokuSolver.hpp"

class ConstraintPropagationSolver : public ISudokuSolver
{
public:

    using Traits   = SudokuTraits;
    using CellKey  = std::pair< int, int>;
    using CellList = std::set< CellKey>;
    using ValueMap = std::map< CellKey, std::vector<int>>;
    using UnitMap  = std::map< CellKey, std::vector<CellList>>;
    using PeerMap  = std::map< CellKey, CellList>;

    ConstraintPropagationSolver();
    ~ConstraintPropagationSolver() override = default;

    auto solve( Traits::Board& board) const -> Traits::BoardResult override;

private:

    mutable std::size_t mRecursions{};

    CellList mCellKeys{}; // All coordinates of the board cells
    UnitMap  mUnits{};    // All units, the cells in the row, column and subsection the cell belongs to
    PeerMap  mPeers{};    // All the peers to the cell. mUnits flattened and without the actual cell

    auto parseGrid( const Traits::Board& board) const -> ValueMap;
    auto assign( ValueMap& valuesMap, const CellKey& key, int value) const -> bool;
    auto eliminate( ValueMap& valuesMap, const CellKey& key, int value) const -> bool;
    auto search( ValueMap valuesMap) const -> ValueMap;
};
