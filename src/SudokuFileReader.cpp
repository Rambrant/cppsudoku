//
//  SudokuFileReader.cpp
//
//  Created by Thomas Rambrant on 2025-04-24.
//

#include <fstream>
#include "SudokuFileReader.hpp"

SudokuFileReader::SudokuFileReader( const std::string& fileName) :
    mInitBoard()
{
    std::ifstream file( fileName);

    if( ! file) 
    {
        throw std::runtime_error( "Could not open file: " + fileName);
    }

    std::size_t row = 0;

    for( std::string line ; std::getline(file, line) ; ++row) 
    {
        if( line.find_first_not_of(" \t\n\v\f\r") == std::string::npos) 
        {
            --row;    // adjust row count
            continue; // skip blank lines
        }

        std::istringstream iss( line);

        std::size_t col = 0;

        for( int value ; iss >> value ; ++col) 
        {
            if( value < 0 || value > Traits::BOARD_SIZE)
            {
                throw std::runtime_error( "Invalid value " + std::to_string( value) + " in row " + std::to_string( row + 1));
            }

            if( row < Traits::BOARD_SIZE && col < Traits::BOARD_SIZE)
            {
                mInitBoard[row][col] = value;
            }
        }

        if( ! iss.eof())
        {
            throw std::runtime_error( "Non-integer found in row: " + std::to_string( row + 1));
        }

        if( col != Traits::BOARD_SIZE) 
        {
            throw std::runtime_error( "Invalid number of columns in row " + std::to_string( row + 1));
        }
    }

    if( row != Traits::BOARD_SIZE) 
    {
        throw std::runtime_error( "Invalid number of rows in file.");
    }
}

//------------------------------------------------------------------------------
//
SudokuFileReader::Traits::Board SudokuFileReader::read() const
{
    return mInitBoard;
}
