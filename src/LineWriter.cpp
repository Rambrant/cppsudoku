//
// Created by Thomas Rambrant on 2025-05-09.
//
#include <ostream>

#include "LineWriter.hpp"

#include "Digit.hpp"

namespace com::rambrant::sudoku
{
    LineWriter::LineWriter( std::ostream& os, const Logger& logger) :
        IWriter( logger),
        mStream( os)
    { }

    auto LineWriter::write( const Traits::Board & board ) const -> void
    {
        for( const auto rowIdx : Traits::INDEX_RANGE)
        {
            for( const auto colIdx : Traits::INDEX_RANGE)
            {
                mStream << valueToDigit<Traits::BOARD_SIZE>( board[rowIdx][colIdx]) << " ";
            }
        }
    }
}
