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
SudokuBoard::SudokuBoard( Board anInitMatrix):
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
bool SudokuBoard::solve( Board& board)
{
    for( int rowIdx : IntRange( 0, BOARD_SIZE))
    {
        for( int colIdx : IntRange( 0, BOARD_SIZE))
        {
            if( board[rowIdx][colIdx] == NO_VALUE)
            {
                for( int value : IntRange( 1, BOARD_SIZE + 1))
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

bool SudokuBoard::isValid( Board& board, int value, int rowPos, int columnPos)
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
        board[rowPos][columnPos] = NO_VALUE;
    }
    
    return result;
}

bool SudokuBoard::rowConstraint( const Board& board, int value, int rowPos)
{
    return check( value, board[ rowPos]);
}

bool SudokuBoard::columnConstraint( const Board& board, int value, int columnPos)
{
    BoardArray columnValues;
    
    std::transform( board.begin(), board.end(), columnValues.begin(),
        [columnPos]( const BoardArray& row){ return row[columnPos]; });
    
    return check( value, columnValues);
}
        
bool SudokuBoard::sectionConstraint( const Board& board, int value, int rowPos, int columnPos)
{
    //
    // Calculate the board coordinates for the top left corner of the section
    //
    int startRow = (rowPos / SUBSECTION_SIZE) * SUBSECTION_SIZE;
    int endRow   = startRow + SUBSECTION_SIZE;
    
    int startCol = (columnPos / SUBSECTION_SIZE) * SUBSECTION_SIZE;
    int endCol   = startCol + SUBSECTION_SIZE;

    //
    // Loop over the section and collect the values
    //
    BoardArray sectionValues;
    
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

bool SudokuBoard::check( int value, const std::array<int, BOARD_SIZE>& arr)
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
