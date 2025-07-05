//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "BlockWriter.hpp"

#include <ostream>

#include "Digit.hpp"

namespace com::rambrant::sudoku
{
    class Logger;

    BlockWriter::BlockWriter( std::ostream& os, const Logger& logger) :
        IWriter( logger),
        mStream( os)
    { }

    auto BlockWriter::write( const Traits::Board & board ) const -> void
    {
        for( const auto rowIdx : Traits::INDEX_RANGE)
        {
            if( rowIdx > 0 && rowIdx % Traits::BOX_SIZE == 0)
            {
                mStream << std::endl; // extra newline between box rows
            }

            for( const auto colIdx : Traits::INDEX_RANGE)
            {
                if( colIdx > 0 && colIdx % Traits::BOX_SIZE == 0)
                {
                    mStream << " "; // extra spacing between box columns
                }

                mStream << valueToDigit<Traits::BOARD_SIZE>( board[rowIdx][colIdx]) << " ";
            }

            mStream << std::endl;
        }
    }
}
