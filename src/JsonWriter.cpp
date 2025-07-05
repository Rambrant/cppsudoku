//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <ostream>
#include <nlohmann/json.hpp>

#include "JsonWriter.hpp"
#include "Digit.hpp"

namespace com::rambrant::sudoku
{
    class Logger;

    JsonWriter::JsonWriter( std::ostream& os, const Logger& logger) :
        IWriter( logger),
        mStream( os)
    { }

    auto JsonWriter::write( const Traits::Board & board ) const -> void
    {
        mStream << "[" << std::endl;

        for( const auto rowIdx : Traits::INDEX_RANGE)
        {
            mStream << "  [ ";

            for( const auto colIdx : Traits::INDEX_RANGE)
            {
                mStream << valueToDigit<Traits::BOARD_SIZE>( board[rowIdx][colIdx]) << (colIdx < Traits::BOARD_SIZE -1 ? ", " : " ");
            }

            mStream << "]" << (rowIdx < Traits::BOARD_SIZE -1 ? "," : "") << std::endl;
        }

        mStream << "]" << std::endl;
    }
}