//
// Created by Thomas Rambrant on 2025-05-09.
//

#include "SudokuLineWriter.hpp"


SudokuLineWriter::SudokuLineWriter( std::ostream& os) :
    mStream( os)
{ }

auto SudokuLineWriter::write( const Traits::Board & board ) const -> void
{
    for( auto rowIdx : Traits::INDEX_RANGE)
    {
        for( auto colIdx : Traits::INDEX_RANGE)
        {
            mStream << valueToChar<Traits::BOARD_SIZE>( board[rowIdx][colIdx]) << " ";
        }
    }
}
