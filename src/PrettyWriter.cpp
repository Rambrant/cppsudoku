//
// Created by Thomas Rambrant on 2025-05-08.
//
#include <ostream>

#include "PrettyWriter.hpp"

#include "Digit.hpp"
#include "Logger.hpp"

namespace com::rambrant::sudoku
{
    PrettyWriter::PrettyWriter( std::ostream& os, const Logger& logger) :
        IWriter( logger),
        mStream( os)
    { }

    auto PrettyWriter::write( const Traits::Board & board ) const -> void
    {
        for( auto rowIdx : Traits::INDEX_RANGE)
        {
            if( rowIdx > 0 && rowIdx % Traits::BOX_SIZE == 0)
            {
                mStream << "------+-------+------" << std::endl; // extra newline between box rows
            }

            for( auto colIdx : Traits::INDEX_RANGE)
            {
                if( colIdx > 0 && colIdx % Traits::BOX_SIZE == 0)
                {
                    mStream << "| "; // extra spacing between box columns
                }

                const Traits::Digit digit = valueToDigit<Traits::BOARD_SIZE>( board[rowIdx][colIdx]);

                mStream << (digit == '0' ? '.' : digit) << " ";
            }

            mStream << std::endl;
        }
    }
}