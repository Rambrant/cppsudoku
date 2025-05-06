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

        CellList rowUnit, colUnit, boxUnit;     // The list of units for every cell

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

        mUnits[key] = { rowUnit, colUnit, boxUnit};     // Add the units for the current cell
        mPeers[key].erase( key);                        // Remove the key from the peers
    }
}

auto ConstraintPropagationSolver::solve( Traits::Board &board) const -> Traits::BoardResult
{
    ValueMap originalValues{ parseGrid( board)};
    ValueMap resultingValues{ search( originalValues)};

    bool result = ! resultingValues.empty();

    //
    // Set the values in the board according the found values
    //
    for( const auto& [key, values] : resultingValues)
    {
        board[key.first][key.second] = values[0];
    }

    return std::make_tuple( result, mRecursions);
}


auto ConstraintPropagationSolver::search( ValueMap valuesMap) const -> ValueMap
{
    if( valuesMap.empty()) return {};

    mRecursions++;

    //
    // Check if we have a solution. The board is solved if all calls has only one value
    //
    bool solved = std::all_of( mCellKeys.begin(), mCellKeys.end(), [&](CellKey key) {
        return valuesMap[key].size() == 1;
    });

    if( solved)
        return valuesMap;

    //
    // Find the cell with the least number of possible values (not taking the already solved ones into account)
    //
    auto const key = *min_element( mCellKeys.begin(), mCellKeys.end(), [&]( const CellKey& lKey, const CellKey& rKey) {
        auto aSize = valuesMap[lKey].size();
        auto bSize = valuesMap[rKey].size();

        if (aSize == 1) aSize = std::numeric_limits<size_t>::max();
        if (bSize == 1) bSize = std::numeric_limits<size_t>::max();

        return aSize < bSize;
    });

    //
    // Try all possible values for the cell, and keep going recursively
    //
    for( const int value : valuesMap[key])
    {
        ValueMap valueClone = valuesMap;

        if( assign( valueClone, key, value))
        {
            auto result = search( valueClone);

            if( ! result.empty())
                return result;
        }
    }

    return {};
}

auto ConstraintPropagationSolver::parseGrid( const Traits::Board & board) const -> ValueMap
{
    ValueMap values;

    //
    // Initialize all cells with all possible values
    //
    for( auto key : mCellKeys)
    {
        auto& vec = values[key];
        vec.insert( vec.end(), Traits::VALUE_RANGE.begin(), Traits::VALUE_RANGE.end());
    }

    //
    // For every cell that has an initial value, assign it
    //
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
    auto& valList = valuesMap[key];     // The possible values for the given key

    //
    // Is the value already eliminated? Then signal success and return early
    //
    if( find( valList.begin(), valList.end(), value) == valList.end())
        return true;

    //
    // Remove the value. If the list becomes empty, something is wrong... return early and signal failure
    //
    valList.erase( remove( valList.begin(), valList.end(), value), valList.end());

    if( valList.empty())
        return false;

    //
    // If there is only one value left, we found a potential solution. Remove that value from all of its peers
    //
    if( valList.size() == 1)
    {
        int lastValue = valList[0];

        for( const auto& peer : mPeers.at(key))
        {
            if( ! eliminate( valuesMap, peer, lastValue))
                return false;
        }
    }

    //
    // We loop over all units, row, column and box (subsection) to check if the value can only go into one place
    //
    for( const auto& unit : mUnits.at(key))
    {
        std::vector<CellKey> places;

        for( const auto& k : unit)
        {
            if( std::find( valuesMap[k].begin(), valuesMap[k].end(), value) != valuesMap[k].end())
                places.push_back(k);
        }

        //
        // The value wasn't found anywhere among the units. This is a contradiction, bail out
        //
        if( places.empty())
            return false;

        //
        // Only one place where the value can go, assign it there
        //
        if( places.size() == 1)
        {
            if( ! assign( valuesMap, places[0], value))
                return false;
        }
    }

    return true;
}

