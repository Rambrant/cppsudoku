//
// Created by Thomas Rambrant on 2025-05-09.
//
#include <ostream>

#include "SudokuBlockWriter.hpp"

namespace com::rambrant::sudoku
{
    SudokuBlockWriter::SudokuBlockWriter( std::ostream& os) :
        mStream( os)
    { }

    auto SudokuBlockWriter::write( const Traits::Board & board ) const -> void
    {
        for( auto rowIdx : Traits::INDEX_RANGE)
        {
            if( rowIdx > 0 && rowIdx % Traits::BOX_SIZE == 0)
            {
                mStream << std::endl; // extra newline between box rows
            }

            for( auto colIdx : Traits::INDEX_RANGE)
            {
                if( colIdx > 0 && colIdx % Traits::BOX_SIZE == 0)
                {
                    mStream << "  "; // extra spacing between box columns
                }

                mStream << valueToChar<Traits::BOARD_SIZE>( board[rowIdx][colIdx]) << " ";
            }

            mStream << std::endl;
        }
    }
}