//
//  Board.cpp
//  Sudoko
//
//  Created by Thomas Rambrant on 2025-04-07.
//

#include "SudokuBoard.hpp"

#include <algorithm>
#include "ISudokuSolver.hpp"
#include "StreamUtil.hpp"

//
// Public methods
//
SudokuBoard::SudokuBoard( Traits::Board anInitMatrix):
    mBoard( std::move( anInitMatrix))
{
}

void SudokuBoard::solve( ISudokuSolver& solver)
{
    solver.solve( mBoard);
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
