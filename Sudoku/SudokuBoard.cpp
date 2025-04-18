//
//  SudokoBoard.hpp
//
//  Created by Thomas Rambrant on 2025-04-07.
//

#include "SudokuBoard.hpp"

#include <algorithm>
#include "ISudokuReader.hpp"
#include "ISudokuSolver.hpp"
#include "StreamUtil.hpp"

//
// Public methods
//
SudokuBoard::SudokuBoard( const ISudokuReader& reader):
    mBoard( reader.read())
{
}

bool SudokuBoard::solve( const ISudokuSolver& solver)
{
    return solver.solve( mBoard);
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
