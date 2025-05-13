//
//  Created by Thomas Rambrant on 2025-04-24.
//
#include <fstream>

#include "SudokuAsciiReader.hpp"
#include "Logger.hpp"

namespace com::rambrant::sudoku
{
    SudokuAsciiReader::SudokuAsciiReader( std::istream& is, const Logger& logger) :
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
        mLogger << Logger::verbose << "Reading detailed verbose log" << std::endl;
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
}