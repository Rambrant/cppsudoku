//
//  Board.cpp
//  Sudoko
//
//  Created by Thomas Rambrant on 2025-04-07.
//

#include "SudokuBoard.hpp"

#include <algorithm>
#include "StreamUtil.hpp"

//
// Public methods
//
SudokuBoard::SudokuBoard( Traits::Board anInitMatrix):
    mBoard( std::move( anInitMatrix))
{
};

void SudokuBoard::solve()
{
    solve( mBoard);
}

//
// Private methods
//
bool SudokuBoard::solve( Traits::Board& board)
{
/*    auto begin = VALUE_RANGE.begin();
    auto end   = VALUE_RANGE.end();
    
    for( auto it = begin; it < end; ++it)
    {
        std::cout << *it;
    }
*/
    for( int rowIdx : Traits::INDEX_RANGE)
    {
        for( int colIdx : Traits::INDEX_RANGE)
        {
            if( board[rowIdx][colIdx] == Traits::NO_VALUE)
            {
                for( int value : Traits::VALUE_RANGE)
                {
                    if( isValid( board, value, rowIdx, colIdx) &&
                        solve( board))
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

bool SudokuBoard::isValid( Traits::Board& board, int value, int rowPos, int columnPos)
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

bool SudokuBoard::rowConstraint( const Traits::Board& board, int value, int rowPos)
{
    return check( value, board[ rowPos]);
}

bool SudokuBoard::columnConstraint( const Traits::Board& board, int value, int columnPos)
{
    Traits::BoardArray columnValues;
    
    std::transform( board.begin(), board.end(), columnValues.begin(),
        [columnPos]( const Traits::BoardArray& row){ return row[columnPos]; });
    
    return check( value, columnValues);
}
        
bool SudokuBoard::sectionConstraint( const Traits::Board& board, int value, int rowPos, int columnPos)
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

bool SudokuBoard::check( int value, const std::array<int, Traits::BOARD_SIZE>& arr)
{
    //
    // Returns true if none of the elements in the array matches the given value
    //
    return std::all_of( arr.begin(), arr.end(),
        [value](int element) { return element != value; }
    );
}

//
// Friend functions
//
std::ostream& operator<<( std::ostream& os, const SudokuBoard& board)
{
    for( const auto& row : board.mBoard)
    {
        std::cout << join( row, ' ') << std::endl;
    }

    return os;
}
