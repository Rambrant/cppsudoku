//
// Created by Thomas Rambrant on 2025-05-09.
//
#include <ostream>

#include "SudokuLineWriter.hpp"

#include "Digit.hpp"

namespace com::rambrant::sudoku
{
    SudokuLineWriter::SudokuLineWriter( std::ostream& os, const Logger& logger) :
        ISudokuWriter( logger),
        mStream( os)
    { }

    auto SudokuLineWriter::write( const Traits::Board & board ) const -> void
    {
        for( auto rowIdx : Traits::INDEX_RANGE)
        {
            for( auto colIdx : Traits::INDEX_RANGE)
            {
                mStream << valueToDigit<Traits::BOARD_SIZE>( board[rowIdx][colIdx]) << " ";
            }
        }
    }
}
