//
// Created by Thomas Rambrant on 2025-05-09.
//
#include <ostream>

#include "BlockWriter.hpp"

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
                    mStream << "  "; // extra spacing between box columns
                }

                mStream << valueToDigit<Traits::BOARD_SIZE>( board[rowIdx][colIdx]) << " ";
            }

            mStream << std::endl;
        }
    }
}