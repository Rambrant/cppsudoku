//
// Created by Thomas Rambrant on 2025-04-29.
//
#include <numeric>

#include "ConstraintPropagationSolver.h"

template<typename RowRange, typename ColRange>
static auto createKeys( const RowRange& rowRange, const ColRange& colRange) -> ConstraintPropagationSolver::CellList
{
    ConstraintPropagationSolver::CellList list;

    for( auto row : rowRange)
    {
        for( auto col: colRange)
        {
            list.emplace( row, col);
        }
    }

    return list;
}

ConstraintPropagationSolver::ConstraintPropagationSolver()
{
    mCellKeys = std::move( createKeys( Traits::INDEX_RANGE, Traits::INDEX_RANGE));

    for( auto key: mCellKeys)
    {
        CellList rowKeys( createKeys( std::vector<int>{key.first}, Traits::INDEX_RANGE));

        mUnits[ key].insert( rowKeys.begin(), rowKeys.end());
        mPeers[ key].merge( rowKeys);

        CellList colKeys( createKeys( Traits::INDEX_RANGE, std::vector{key.second}));

        mUnits[ key].insert( colKeys.begin(), colKeys.end());
        mPeers[ key].merge( colKeys);

        CellList subKeys( createSubsectionKeys( key));

        mUnits[ key].insert( subKeys.begin(), subKeys.end());
        mPeers[ key].merge( subKeys);

        mPeers[key].erase( key);
    }
}

auto ConstraintPropagationSolver::solve( Traits::Board &board) const -> Traits::BoardResult
{
    bool result = solver( board);

    return std::make_tuple( result, mRecursions);
}

auto ConstraintPropagationSolver::createSubsectionKeys( const CellKey &key) -> CellList
{
    CellList list{};

    int rowStart = (key.first  / Traits::SUBSECTION_SIZE) * Traits::SUBSECTION_SIZE;
    int colStart = (key.second / Traits::SUBSECTION_SIZE) * Traits::SUBSECTION_SIZE;

    for( int rowIdx = 0; rowIdx < Traits::SUBSECTION_SIZE; ++rowIdx)
    {
        for( int colIdx = 0; colIdx < Traits::SUBSECTION_SIZE; ++colIdx)
        {
            list.emplace( rowStart + rowIdx, colStart + colIdx);
        }
    }

    return list;
}

bool ConstraintPropagationSolver::solver(Traits::Board &board) const
{
     return true;
}

