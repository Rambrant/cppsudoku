//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
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
