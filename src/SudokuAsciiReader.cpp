//
//  SudokuFileReader.cpp
//
//  Created by Thomas Rambrant on 2025-04-24.
//

#include <fstream>
#include <regex>

#include "SudokuAsciiReader.hpp"

SudokuAsciiReader::SudokuAsciiReader( std::istream& is, com::rambrant::sudoku::Logger& logger) :
    ISudokuReader( logger),
    mStream( is)
{
}

auto SudokuAsciiReader::read() const -> Traits::Board
{
    constexpr int MAX_VALUES = Traits::BOARD_SIZE * Traits::BOARD_SIZE;

    Traits::Board board{};

    int  count = 0;
    char ch;

    mLogger << "Reading board" << std::endl;
    mLogger << com::rambrant::sudoku::Logger::verbose << "Reading detailed verbose log" << std::endl;
    mLogger << "back to defaul" << std::endl;

    while( count < MAX_VALUES && mStream.get( ch))
    {
        if( ch == '.') ch = '0';

        int value = charToValue<Traits::BOARD_SIZE>( ch);

        if (value != -1)
        {
            board[count / Traits::BOARD_SIZE][count % Traits::BOARD_SIZE] = value;
            ++count;
        }
    }

    if( count != MAX_VALUES)
    {
        throw std::runtime_error( "Not enough values to fill the Sudoku board.");
    }

    return board;
}
