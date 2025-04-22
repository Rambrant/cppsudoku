//
//  SudokoBoard.hpp
//
//  Created by Thomas Rambrant on 2025-04-07.
//

#include "SudokuBoard.hpp"

#include "ISudokuReader.hpp"
#include "ISudokuSolver.hpp"
#include "StreamUtil.hpp"

//------------------------------------------------------------------------------
//
SudokuBoard::SudokuBoard( const ISudokuReader& reader):
    mBoard( reader.read())
{
}

//------------------------------------------------------------------------------
//
bool SudokuBoard::solve( const ISudokuSolver& solver)
{
    return solver.solve( mBoard);
}

//------------------------------------------------------------------------------
//
// Friend functions
//
std::ostream& operator<<( std::ostream& os, const SudokuBoard& board)
{
    using Traits = SudokuTraits;

    for( auto rowIdx : Traits::INDEX_RANGE)
    {
        if( rowIdx > 0 && rowIdx % Traits::SUBSECTION_SIZE == 0)
        {
            os << std::endl; // extra newline between subsections rows
        }
        
        for( auto colIdx : Traits::INDEX_RANGE)
        {
            if( colIdx > 0 && colIdx % Traits::SUBSECTION_SIZE == 0)
            {
                os << " "; // extra spacing between subsection columns
            }

            os << board.mBoard[rowIdx][colIdx] << " ";
        }
        
        os << std::endl;
    }

    return os;
}
