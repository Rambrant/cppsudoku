//
// Created by Thomas Rambrant on 2025-04-29.
//
#include "ConstraintPropagationSolver.hpp"

#include <numeric>

namespace com::rambrant::sudoku
{
    ConstraintPropagationSolver::ConstraintPropagationSolver( const Logger& logger) :
        ISudokuSolver( logger)
    {
        //
        // Init all the square coordinates
        //
        for( auto row : Traits::INDEX_RANGE)
        {
            for( auto column : Traits::INDEX_RANGE)
            {
                mSquares.emplace( row, column);
            }
        }

        //
        // For all existing squares init all units and peers
        //
        for( auto square: mSquares)
        {
            auto [rowIdx, colIdx] = square;

            Squares rowUnit, colUnit, boxUnit;

            //
            // Init the row unit
            //
            for ( auto column : Traits::INDEX_RANGE)
            {
                rowUnit.emplace( rowIdx, column);
                mPeers[square].emplace( rowIdx, column);
            }

            //
            // Init the column unit
            //
            for ( auto row : Traits::INDEX_RANGE)
            {
                colUnit.emplace( row, colIdx);
                mPeers[square].emplace( row, colIdx);
            }

            //
            // Init the box unit
            //
            int boxRowStart = (rowIdx / Traits::BOX_SIZE) * Traits::BOX_SIZE;
            int boxColStart = (colIdx / Traits::BOX_SIZE) * Traits::BOX_SIZE;

            for( int row = boxRowStart; row < boxRowStart + Traits::BOX_SIZE; ++row)
            {
                for( int column = boxColStart; column < boxColStart + Traits::BOX_SIZE; ++column)
                {
                    boxUnit.emplace( row, column);
                    mPeers[square].emplace( row, column);
                }
            }

            mUnits[square] = { rowUnit, colUnit, boxUnit};     // Add the units for the current square
            mPeers[square].erase( square);                     // Remove the current square from the collected peers
        }
    }

    auto ConstraintPropagationSolver::solve( Traits::Board &board) const -> Traits::BoardResult
    {
        SquareValues originalValues{ parseGrid( board)};
        SquareValues resultingValues{ search( originalValues)};

        bool result = ! resultingValues.empty();

        //
        // Set the found values back into the board
        //
        for( const auto& [key, values] : resultingValues)
        {
            board[key.first][key.second] = values[0];   // The values for the square are guarantied to be just one...
        }

        return std::make_tuple( result, mRecursions);
    }


    auto ConstraintPropagationSolver::search( SquareValues allValues) const -> SquareValues
    {
        if( allValues.empty()) return {};

        mRecursions++;

        //
        // Check if we have a solution. The board is solved if all calls have only one value
        //
        bool solved = std::all_of( mSquares.begin(), mSquares.end(), [&](Square key) {
            return allValues[key].size() == 1;
        });

        if( solved)
            return allValues;

        //
        // Find the cell with the least number of possible values (not taking the already solved ones into account)
        //
        auto const key = *min_element( mSquares.begin(), mSquares.end(), [&]( const Square& lSquare, const Square& rSquare) {
            auto lSize = allValues[lSquare].size();
            auto rSize = allValues[rSquare].size();

            if( lSize == 1) lSize = std::numeric_limits<size_t>::max();
            if( rSize == 1) rSize = std::numeric_limits<size_t>::max();

            return lSize < rSize;
        });

        //
        // Try all possible values for the cell, and keep going recursively
        //
        for( const int value : allValues[key])
        {
            SquareValues valueClone = allValues;

            if( assign( valueClone, key, value))
            {
                auto result = search( valueClone);

                if( ! result.empty())
                    return result;
            }
        }

        return {};
    }

    auto ConstraintPropagationSolver::parseGrid( const Traits::Board & board) const -> SquareValues
    {
        SquareValues values;

        //
        // Initialize all cells with all possible values
        //
        for( auto key : mSquares)
        {
            auto& vec = values[key];
            vec.insert( vec.end(), Traits::VALUE_RANGE.begin(), Traits::VALUE_RANGE.end());
        }

        //
        // For every cell that has an initial value, assign it
        //
        for( auto [ rowIdx, colIdx ] : mSquares)
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

    auto ConstraintPropagationSolver::assign( SquareValues& allValues, const Square& square, const int value) const -> bool
    {
        //
        // Collect other values to eliminate
        //
        auto& squareValues = allValues[square];

        std::vector<int> otherValues;
        std::copy_if( squareValues.begin(), squareValues.end(), std::back_inserter( otherValues), [&](int v) { return v != value; });

        //
        // Eliminate all remaining values
        //
        for( const auto otherValue : otherValues)
        {
            if( ! eliminate( allValues, square, otherValue))
            {
                return false;
            }
        }

        return true;
    }

    auto ConstraintPropagationSolver::eliminate( SquareValues& allValues, const Square& square, const int value) const -> bool
    {
        auto& squareValues = allValues[square];     // The possible values for the given key

        //
        // Is the value already eliminated? Then signal success and return early
        //
        if( find( squareValues.begin(), squareValues.end(), value) == squareValues.end())
            return true;

        //
        // Remove the value. If the list becomes empty, something is wrong... return early and signal failure
        //
        squareValues.erase( remove( squareValues.begin(), squareValues.end(), value), squareValues.end());

        if( squareValues.empty())
            return false;

        //
        // If there is only one value left, we found a potential solution. Remove that value from all of its peers
        //
        if( squareValues.size() == 1)
        {
            int lastValue = squareValues[0];

            for( const auto& peer : mPeers.at(square))
            {
                if( ! eliminate( allValues, peer, lastValue))
                    return false;
            }
        }

        //
        // We loop over all units, row, column and box to check if the value can only go into one place
        //
        for( const auto& unit : mUnits.at(square))
        {
            std::vector<Square> places;

            for( const auto& k : unit)
            {
                if( std::find( allValues[k].begin(), allValues[k].end(), value) != allValues[k].end())
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
                if( ! assign( allValues, places[0], value))
                    return false;
            }
        }

        return true;
    }
}
