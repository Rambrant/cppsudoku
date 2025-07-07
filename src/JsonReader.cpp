//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "JsonReader.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

#include "Digit.hpp"

namespace com::rambrant::sudoku
{
    JsonReader::JsonReader( std::istream& is, const Logger& logger) :
        IReader( logger),
        mStream( is)
    {}

    auto JsonReader::read() const -> Traits::Board
    {
        Traits::Board board{};

        nlohmann::json json;

        mStream >> json;

        if( ! json.is_array() || json.size() != Traits::BOARD_SIZE)
        {
            throw std::runtime_error( "Wrong number of rows in JSON file.");
        }

        for( const int rowIdx : Traits::INDEX_RANGE)
        {
            const auto& rowArray = json[rowIdx];

            if( ! rowArray.is_array() || rowArray.size() != Traits::BOARD_SIZE)
            {
                throw std::runtime_error( "Wrong number of columns in JSON file.");
            }

            for( const int colIdx : Traits::INDEX_RANGE)
            {
                Traits::Value value{};
                auto&         cell = rowArray[colIdx];

                if( cell.is_number_integer())
                {
                    value = cell.get<int>();
                }
                else
                {
                    value = digitToValue<Traits::BOARD_SIZE>( cell.get<std::string>());
                }

                if( value < 0 || value > Traits::MAX_VALUE)
                {
                    throw std::runtime_error( "Invalid value '" + cell.dump() + "' in JSON file.");
                }

                board[rowIdx][colIdx] = value;
            }
        }

        return board;
    }
}