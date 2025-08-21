//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "BackTrackingSolver.hpp"

#include <algorithm>
#include <tuple>
#include <tuple>
#include <tuple>

#include "Logger.hpp"
#include "RangeView.hpp"
#include "SudokuBoard.hpp"

namespace com::rambrant::sudoku
{
    namespace detail
    {
        //
        // Helper function implementations
        //
        auto checkValue( Traits::Value value, const Traits::BoardArray & unitValues ) -> bool
        {
            //
            // Returns true if none of the elements in the array matches the given value
            //
            return std::all_of( unitValues.begin(), unitValues.end(),
                [value]( const int element) { return element != value; }
            );
        }

        auto rowConstraint( const Traits::Board & board, const Traits::Value value, const int rowPos ) -> bool
        {
            return checkValue( value, board[ rowPos]);
        }

        auto columnConstraint( const Traits::Board & board, const Traits::Value value, const int columnPos ) -> bool
        {
            Traits::BoardArray columnValues;

            std::transform( board.begin(), board.end(), columnValues.begin(),
                [columnPos]( const Traits::BoardArray& row){ return row[columnPos]; });

            return checkValue( value, columnValues);
        }

        auto boxConstraint( const Traits::Board & board, const Traits::Value value, const int rowPos, const int columnPos ) -> bool
        {
            //
            // Calculate the board coordinates for the top left corner of the box
            //
            const int startRow = (rowPos / Traits::BOX_SIZE) * Traits::BOX_SIZE;
            const int endRow   = startRow + Traits::BOX_SIZE;

            const int startCol = (columnPos / Traits::BOX_SIZE) * Traits::BOX_SIZE;
            const int endCol   = startCol + Traits::BOX_SIZE;

            //
            // Loop over the box squares and collect the values
            //
            Traits::BoardArray boxValues;

            auto boxIter = boxValues.begin();

            for( const auto& row : RangeView( board, startRow, endRow))
            {
                for( const int  squareValue : RangeView( row, startCol, endCol))
                {
                    *boxIter =  squareValue;

                    boxIter++;
                }
            }

            return checkValue( value, boxValues);
        }

        auto setValid( Traits::Board& board, const Traits::Value value, const int rowPos, const int columnPos ) -> bool
        {
            const bool result = rowConstraint( board, value, rowPos)            &&
                                columnConstraint( board, value, columnPos)      &&
                                boxConstraint( board, value, rowPos, columnPos);

            if( result)
            {
                //
                // Valid guess, set the value
                //
                board[rowPos][columnPos] = value;
            }
            else
            {
                //
                // non-valid guess, back of
                //
                board[rowPos][columnPos] = Traits::NO_VALUE;
            }

            return result;
        }

        auto search( Traits::Board & board, int& recursions, std::atomic<bool>& cancelFlag) -> bool // NOLINT(misc-no-recursion)
{
            if( cancelFlag.load())
            {
                throw CancelledException{}; // Exit early
            }

            ++recursions;

            for( const int rowIdx : Traits::INDEX_RANGE)
            {
                for( const int colIdx : Traits::INDEX_RANGE)
                {
                    if( board[rowIdx][colIdx] == Traits::NO_VALUE)
                    {
                        for( const int value : Traits::VALUE_RANGE)
                        {
                            if( setValid( board, value, rowIdx, colIdx) &&
                                search( board, recursions, cancelFlag))
                            {
                                return true;
                            }
                        }

                        return false;
                    }
                }
            }

            return true;
        }
    }

    //
    // Member functions
    //
    BackTrackingSolver::BackTrackingSolver( const Logger & logger) :
        ISolver( logger)
    {}

    auto BackTrackingSolver::solve( Traits::Board& board, std::atomic<bool>& cancelFlag ) const -> Traits::BoardResult
    {
        int recursions{ 0};

        try
        {
            bool result = detail::search( board, recursions, cancelFlag);

            if( result)
                cancelFlag.store( true);    // Terminate any other solver prematurely

            return std::make_tuple( result, recursions, board);
        }
        catch( const CancelledException&)
        {
            //
            // Returned prematurely
            //
            return std::make_tuple( false, recursions, Traits::Board{});
        }
    }
}
