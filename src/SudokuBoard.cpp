//
//  SudokoBoard.hpp
//
//  Created by Thomas Rambrant on 2025-04-07.
//

#include "SudokuBoard.hpp"

#include "ISudokuReader.hpp"
#include "ISudokuSolver.hpp"
#include "SudokuUtil.hpp"

SudokuBoard::SudokuBoard( const ISudokuReader& reader):
    mBoard( reader.read())
{
}

auto SudokuBoard::solve( const ISudokuSolver& solver) -> Traits::BoardResult
{
    return solver.solve( mBoard);
}

std::ostream& operator<<( std::ostream& os, const SudokuBoard& board)
{
    using Traits = SudokuTraits;

    for( auto rowIdx : Traits::INDEX_RANGE)
    {
        if( rowIdx > 0 && rowIdx % Traits::BOX_SIZE == 0)
        {
            os << std::endl; // extra newline between box rows
        }
        
        for( auto colIdx : Traits::INDEX_RANGE)
        {
            if( colIdx > 0 && colIdx % Traits::BOX_SIZE == 0)
            {
                os << " "; // extra spacing between box columns
            }

            os << board.mBoard[rowIdx][colIdx] << " ";
        }
        
        os << std::endl;
    }

    return os;
}
