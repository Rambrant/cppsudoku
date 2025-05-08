//
// Created by Thomas Rambrant on 2025-05-08.

#include <ostream>

#include "SudokuPrettyWriter.hpp"


SudokuPrettyWriter::SudokuPrettyWriter( std::ostream& os) :
    mStream( os)
{ }

auto SudokuPrettyWriter::write( const Traits::Board & board ) const -> void
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

            std::string val = std::to_string( board[rowIdx][colIdx]);

            mStream << (val == "0" ? "." : val) << " ";
        }

        mStream << std::endl;
    }
}
