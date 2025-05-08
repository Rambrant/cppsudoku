//
//  SudokuFileReader.cpp
//
//  Created by Thomas Rambrant on 2025-04-24.
//

#include <fstream>
#include <regex>

#include "SudokuAsciiReader.hpp"

SudokuAsciiReader::SudokuAsciiReader( std::istream& is) :
    mStream( is)
{
}

auto SudokuAsciiReader::read() const -> Traits::Board
{
    Traits::Board board;

    std::string line;
    std::size_t row = 0;

    while( row < Traits::BOARD_SIZE && std::getline( mStream, line))
    {
        //
        // Replace '.' with zeros and replace everything else that isn't a number with space
        //
        std::replace( line.begin(), line.end(), '.', '0');
        line = std::regex_replace( line, std::regex("[^0-9 ]"), " ");

        //
        // If the resulting line is just white-space, skip it
        //
        if( line.find_first_not_of( " \t\n\v\f\r") == std::string::npos)
            continue;

        //
        // Read all found integers and check
        std::istringstream iss( line);

        for( int col = 0, value; iss >> value; ++col)
        {
            if( value < 0 || value > Traits::MAX_VALUE)
            {
                throw std::runtime_error( "Invalid value " + std::to_string( value) + " in row " + std::to_string( row + 1));
            }

            if( col < Traits::BOARD_SIZE)
            {
                board[row][col] = value;
            }
            else
            {
                throw std::runtime_error( "Invalid number of columns in row " + std::to_string( row + 1));
            }
        }

        ++row;
    }

    if( row != Traits::BOARD_SIZE)
    {
        throw std::runtime_error( "Invalid number of columns in row " + std::to_string( row + 1));
    }

    return board;;
}
