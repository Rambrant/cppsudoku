//
//  BackTrackingSolver.cpp
//
//  Created by Thomas Rambrant on 2025-04-12.
//

#include "SudokuBoard.hpp"

#include <algorithm>
#include "BackTrackingSolver.hpp"

//------------------------------------------------------------------------------
//
BackTrackingSolver::BackTrackingSolver() :
    mRecursions{}
{ }

//------------------------------------------------------------------------------
//
auto BackTrackingSolver::solve(Traits::Board &board) const -> Traits::BoardResult
{
    bool result = solver( board);

    return std::make_tuple( result, mRecursions);
}

//------------------------------------------------------------------------------
//
bool BackTrackingSolver::solver( Traits::Board &board) const // NOLINT(misc-no-recursion)
{
    ++mRecursions;

    for( int rowIdx : Traits::INDEX_RANGE)
    {
        for( int colIdx : Traits::INDEX_RANGE)
        {
            if( board[rowIdx][colIdx] == Traits::NO_VALUE)
            {
                for( int value : Traits::VALUE_RANGE)
                {
                    if( isValid( board, value, rowIdx, colIdx) &&
                        solver( board))
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

//------------------------------------------------------------------------------
//
bool BackTrackingSolver::isValid( Traits::Board& board, int value, int rowPos, int columnPos) const
{
    bool result = rowConstraint( board, value, rowPos)                  &&
                  columnConstraint( board, value, columnPos)            &&
                  sectionConstraint( board, value, rowPos, columnPos);
                  
    if( result)
    {
        board[rowPos][columnPos] = value;
    }
    else
    {
        board[rowPos][columnPos] = Traits::NO_VALUE;
    }
    
    return result;
}

//------------------------------------------------------------------------------
//
bool BackTrackingSolver::rowConstraint( const Traits::Board& board, int value, int rowPos) const
{
    return check( value, board[ rowPos]);
}

//------------------------------------------------------------------------------
//
bool BackTrackingSolver::columnConstraint( const Traits::Board& board, int value, int columnPos) const
{
    Traits::BoardArray columnValues;
    
    std::transform( board.begin(), board.end(), columnValues.begin(),
        [columnPos]( const Traits::BoardArray& row){ return row[columnPos]; });
    
    return check( value, columnValues);
}
        
//------------------------------------------------------------------------------
//
bool BackTrackingSolver::sectionConstraint( const Traits::Board& board, int value, int rowPos, int columnPos) const
{
    //
    // Calculate the board coordinates for the top left corner of the section
    //
    int startRow = (rowPos / Traits::SUBSECTION_SIZE) * Traits::SUBSECTION_SIZE;
    int endRow   = startRow + Traits::SUBSECTION_SIZE;
    
    int startCol = (columnPos / Traits::SUBSECTION_SIZE) * Traits::SUBSECTION_SIZE;
    int endCol   = startCol + Traits::SUBSECTION_SIZE;

    //
    // Loop over the section and collect the values
    //
    Traits::BoardArray sectionValues;
    
    auto secIter = sectionValues.begin();
    
    for( const auto& row : subrangeView( board, startRow, endRow))
    {
        for( int cell : subrangeView( row, startCol, endCol))
        {
            *secIter = cell;
            
            secIter++;
        }
    }
    
    return check( value, sectionValues);
}

//------------------------------------------------------------------------------
//
bool BackTrackingSolver::check( int value, const std::array<int, Traits::BOARD_SIZE>& arr) const
{
    //
    // Returns true if none of the elements in the array matches the given value
    //
    return std::all_of( arr.begin(), arr.end(),
        [value](int element) { return element != value; }
    );
}

