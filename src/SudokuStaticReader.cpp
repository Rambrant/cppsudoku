//
//  SudokuStaticReader.cpp
//  Sudoku
//
//  Created by Thomas Rambrant on 2025-04-18.
//

#include "SudokuStaticReader.hpp"


//------------------------------------------------------------------------------
//
SudokuStaticReader::SudokuStaticReader( const Traits::Board& board) :
    mInitBoard( std::move(board))
{
}
        
//------------------------------------------------------------------------------
//
SudokuStaticReader::Traits::Board SudokuStaticReader::read() const
{
    return mInitBoard;
}
