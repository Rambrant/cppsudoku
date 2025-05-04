//
// Created by Thomas Rambrant on 2025-04-29.
//
#include <numeric>

#include "ConstraintPropagationSolver.h"


ConstraintPropagationSolver::ConstraintPropagationSolver()
{
    //
    // Init all the cell keys
    //
    for( auto r : Traits::INDEX_RANGE)
    {
        for( auto c : Traits::INDEX_RANGE)
        {
            mCellKeys.emplace( r, c);
        }
    }

    //
    // For all existing keys init all units and peers
    //
    for( auto key: mCellKeys)
    {
        auto [rowIdx, colIdx] = key;

        CellList rowUnit, colUnit, boxUnit;

        for ( auto c : Traits::INDEX_RANGE)
        {
            rowUnit.emplace( rowIdx, c);
            mPeers[key].emplace( rowIdx, c);
        }

        for ( auto r : Traits::INDEX_RANGE)
        {
            colUnit.emplace( r, colIdx);
            mPeers[key].emplace( r, colIdx);
        }

        int boxRowStart = (rowIdx / Traits::SUBSECTION_SIZE) * Traits::SUBSECTION_SIZE;
        int boxColStart = (colIdx / Traits::SUBSECTION_SIZE) * Traits::SUBSECTION_SIZE;

        for( int r = boxRowStart; r < boxRowStart + Traits::SUBSECTION_SIZE; ++r)
        {
            for( int c = boxColStart; c < boxColStart + Traits::SUBSECTION_SIZE; ++c)
            {
                boxUnit.emplace( r, c);
                mPeers[key].emplace( r, c);
            }
        }

        mUnits[key] = { rowUnit, colUnit, boxUnit};
        mPeers[key].erase( key);    // Remove the key from the peers
    }
}

auto ConstraintPropagationSolver::solve( Traits::Board &board) const -> Traits::BoardResult
{
    ValueMap values{ parseGrid( board)};

    bool result = search( values);

    return std::make_tuple( result, mRecursions);
}


auto ConstraintPropagationSolver::search( ValueMap valuesMap) const -> ValueMap
{
    return true;
}

auto ConstraintPropagationSolver::parseGrid( const Traits::Board & board) const -> ValueMap
{
    ValueMap values;

    for( auto key : mCellKeys)
    {
        auto& vec = values[key];
        vec.insert( vec.end(), Traits::VALUE_RANGE.begin(), Traits::VALUE_RANGE.end());
    }

    for( auto [ rowIdx, colIdx ] : mCellKeys)
    {
        auto value = board[ rowIdx][ colIdx];

        if( value != Traits::NO_VALUE)
        {
            if( ! assign( values, { rowIdx, colIdx }, value))
            {
                std::string key = "[" + std::to_string( rowIdx) + "," + std::to_string( colIdx) + "]";
                throw std::runtime_error("Illegal board: contradiction at [" + key + "] for digit " + std::to_string(value));
            }
        }
    }

    return values;
}

auto ConstraintPropagationSolver::assign( ValueMap& valuesMap, const CellKey& key, const int value) const -> bool
{
    //
    // Collect other digits to eliminate
    //
    auto& cellValues = valuesMap[key];

    std::vector<int> otherValues;
    std::copy_if( cellValues.begin(), cellValues.end(), std::back_inserter( otherValues), [&](int d) { return d != value; });

    //
    // Eliminate all remaining digits
    //
    for( const auto otherValue : otherValues)
    {
        if( ! eliminate( valuesMap, key, otherValue))
        {
            return false;
        }
    }

    return true;
}

auto ConstraintPropagationSolver::eliminate( ValueMap& valuesMap, const CellKey& key, const int value) const -> bool
{
    auto& valList = valuesMap[key];

    if( find( valList.begin(), valList.end(), value) == valList.end())
        return true;

    valList.erase( remove( valList.begin(), valList.end(), value), valList.end());

    if( valList.empty()) return false;

    if( valList.size() == 1)
    {
        int lastValue = valList[0];

        for( const auto& peer : mPeers.at(key))
        {
            if( ! eliminate( valuesMap, peer, lastValue))
                return false;
        }
    }

    for( const auto& unit : mUnits.at(key))
    {
        std::vector<CellKey> places;

        for( const auto& s : unit)
        {
            if( std::find( valuesMap[s].begin(), valuesMap[s].end(), value) != valuesMap[s].end())
                places.push_back(s);
        }

        if( places.empty())
            return false;

        if( places.size() == 1)
        {
            if( ! assign( valuesMap, places[0], value))
                return false;
        }
    }

    return true;
}

