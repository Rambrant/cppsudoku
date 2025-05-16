//
//  Created by Thomas Rambrant on 2025-04-12.
//
#include "BackTrackingSolver.hpp"
#include "SudokuBoard.hpp"

#include <tuple>

namespace com::rambrant::sudoku
{
    namespace
    {
        //
        // Helper functions
        //
        auto check( int value, const Traits::BoardArray & unitValues ) -> bool
        {
            //
            // Returns true if none of the elements in the array matches the given value
            //
            return std::all_of( unitValues.begin(), unitValues.end(),
                [value](int element) { return element != value; }
            );
        }

        auto rowConstraint( const Traits::Board & board, int value, int rowPos ) -> bool
        {
            return check( value, board[ rowPos]);
        }

        auto columnConstraint( const Traits::Board & board, int value, int columnPos ) -> bool
        {
            Traits::BoardArray columnValues;

            std::transform( board.begin(), board.end(), columnValues.begin(),
                [columnPos]( const Traits::BoardArray& row){ return row[columnPos]; });

            return check( value, columnValues);
        }

        auto boxConstraint( const Traits::Board & board, int value, int rowPos, int columnPos ) -> bool
        {
            //
            // Calculate the board coordinates for the top left corner of the box
            //
            int startRow = (rowPos / Traits::BOX_SIZE) * Traits::BOX_SIZE;
            int endRow   = startRow + Traits::BOX_SIZE;

            int startCol = (columnPos / Traits::BOX_SIZE) * Traits::BOX_SIZE;
            int endCol   = startCol + Traits::BOX_SIZE;

            //
            // Loop over the box squares and collect the values
            //
            Traits::BoardArray boxValues;

            auto boxIter = boxValues.begin();

            for( const auto& row : subrangeView( board, startRow, endRow))
            {
                for( int cell : subrangeView( row, startCol, endCol))
                {
                    *boxIter = cell;

                    boxIter++;
                }
            }

            return check( value, boxValues);
        }

        auto isValid( Traits::Board& board, int value, int rowPos, int columnPos ) -> bool
        {
            bool result = rowConstraint( board, value, rowPos)                  &&
                          columnConstraint( board, value, columnPos)            &&
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

        auto search( Traits::Board & board, int& recursions) -> bool // NOLINT(misc-no-recursion)
{
            ++recursions;

            for( int rowIdx : Traits::INDEX_RANGE)
            {
                for( int colIdx : Traits::INDEX_RANGE)
                {
                    if( board[rowIdx][colIdx] == Traits::NO_VALUE)
                    {
                        for( int value : Traits::VALUE_RANGE)
                        {
                            if( isValid( board, value, rowIdx, colIdx) &&
                                search( board, recursions))
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
        ISudokuSolver( logger)
    {}

    auto BackTrackingSolver::solve( Traits::Board &board) const -> Traits::BoardResult
    {
        int  recursions{ 0};

        bool result = search( board, recursions);

        return std::make_tuple( result, recursions);
    }
}
