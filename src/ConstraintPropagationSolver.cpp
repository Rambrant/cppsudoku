//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "ConstraintPropagationSolver.hpp"

#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <vector>

namespace com::rambrant::sudoku
{
    namespace
    {
        //
        // The control structure defining the squares, units and peers for the Sudoku board
        //
        using Square        = std::pair<int, int>;                          // Row and column index
        using Squares       = std::set<Square>;                             // All squares on the board
        using SquareValues  = std::map<Square, std::vector<Traits::Value>>; // Map from each square to its current list of possible values.
        using Units         = std::map<Square, std::vector<Squares>>;       // Map from each square to its units, row, column and box.
        using Peers         = std::map<Square, Squares>;                    // Map from each square to its peers.

        struct BoardStructure
        {
            Squares mSquares{};
            Units   mUnits{};
            Peers   mPeers{};

            BoardStructure();
        };

        const BoardStructure gBoardsStructure;

        BoardStructure::BoardStructure()
        {
            //
            // Init all the square coordinates
            //
            for( const auto row : Traits::INDEX_RANGE)
            {
                for( const auto column : Traits::INDEX_RANGE)
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
                const int boxRowStart = (rowIdx / Traits::BOX_SIZE) * Traits::BOX_SIZE;
                const int boxColStart = (colIdx / Traits::BOX_SIZE) * Traits::BOX_SIZE;

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

        //
        // Helper functions
        //
        auto eliminate( SquareValues& allValues, const Square& square, Traits::Value value ) -> bool;
        auto assign( SquareValues& allValues, const Square& square, Traits::Value value ) -> bool;
        auto search( SquareValues allValues, int& recursions) -> SquareValues;
        auto parseGrid( const Traits::Board & board) -> SquareValues;

        //
        // Helper function implementations
        //
        auto eliminate( SquareValues& allValues, const Square& square, const Traits::Value value ) -> bool // NOLINT(misc-no-recursion)
        {
            auto& squareValues = allValues[square];     // The possible values for the given key

            //
            // Is the value already eliminated? Then signal success and return early
            //
            if( std::find( squareValues.begin(), squareValues.end(), value) == squareValues.end())
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
                const Traits::Value lastValue = squareValues[0];

                for( const auto& peer : gBoardsStructure.mPeers.at(square))
                {
                    if( ! eliminate( allValues, peer, lastValue))
                        return false;
                }
            }

            //
            // We loop over all units, row, column and box to check if the value can only go into one place
            //
            for( const auto& unit : gBoardsStructure.mUnits.at(square))
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

        auto assign( SquareValues& allValues, const Square& square, const Traits::Value value ) -> bool // NOLINT(misc-no-recursion)
        {
            //
            // Collect other values to eliminate
            //
            auto& squareValues = allValues[square];

            std::vector<Traits::Value> otherValues;
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

        auto search( SquareValues allValues, int& recursions ) -> SquareValues // NOLINT(misc-no-recursion)
        {
            if( allValues.empty()) return {};

            recursions++;

            //
            // Check if we have a solution. The board is solved if all calls have only one value
            //
            bool solved = std::all_of( gBoardsStructure.mSquares.begin(), gBoardsStructure.mSquares.end(), [&]( const Square& key)
            {
                return allValues[key].size() == 1;
            });

            if( solved)
                return allValues;

            //
            // Find the cell with the least number of possible values (not taking the already solved ones into account)
            //
            auto const key = *std::min_element( gBoardsStructure.mSquares.begin(), gBoardsStructure.mSquares.end(), [&]( const Square& lSquare, const Square& rSquare)
            {
                auto lSize = allValues[lSquare].size();
                auto rSize = allValues[rSquare].size();

                if( lSize == 1) lSize = std::numeric_limits<size_t>::max();
                if( rSize == 1) rSize = std::numeric_limits<size_t>::max();

                return lSize < rSize;
            });

            //
            // Try all possible values for the cell and keep going recursively
            //
            for( const Traits::Value value : allValues[key])
            {
                if( SquareValues valueClone = allValues; assign( valueClone, key, value))
                {
                    if( auto result = search( valueClone, recursions); ! result.empty())
                        return result;
                }
            }

            return {};
        }

        auto parseGrid( const Traits::Board & board) -> SquareValues
        {
            SquareValues values;

            //
            // Initialize all cells with all possible values
            //
            for( auto square : gBoardsStructure.mSquares)
            {
                auto& squareValues = values[square];
                squareValues.insert( squareValues.end(), Traits::VALUE_RANGE.begin(), Traits::VALUE_RANGE.end());
            }

            //
            // For every square that has an initial value, assign it
            //
            for( auto [ rowIdx, colIdx ] : gBoardsStructure.mSquares)
            {
                if( const auto value = board[ rowIdx][ colIdx]; value != Traits::NO_VALUE)
                {
                    if( ! assign( values, { rowIdx, colIdx }, value))
                    {
                        const std::string square = "[" + std::to_string( rowIdx) + "," + std::to_string( colIdx) + "]";
                        throw std::runtime_error("Illegal board: contradiction at [" + square + "] for digit " + std::to_string(value));
                    }
                }
            }

            return values;
        }
    }

    //
    // Member functions
    //
    ConstraintPropagationSolver::ConstraintPropagationSolver( const Logger& logger) :
        ISolver( logger)
    {}

    auto ConstraintPropagationSolver::solve( Traits::Board& board) const -> Traits::BoardResult
    {
        int  recursions{ 0};
        bool result{ false};

        try
        {
            const SquareValues originalValues{ parseGrid( board)};
            const SquareValues resultingValues{ search( originalValues, recursions)};

            result = ! resultingValues.empty();

            //
            // Set the found values back into the board
            //
            for( const auto& [square, values] : resultingValues)
            {
                board[square.first][square.second] = values[0];   // The values for the square are guarantied to be just one...
            }
        }
        catch( const std::exception& e)
        {
            std::cerr << e.what() << std::endl;

            result = false;
        }

        return std::make_tuple( result, recursions);
    }
}
