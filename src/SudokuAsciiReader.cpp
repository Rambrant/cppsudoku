//
//  Created by Thomas Rambrant on 2025-04-24.
//
#include <fstream>

#include "SudokuAsciiReader.hpp"

#include "Digit.hpp"
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
        char digit;

        while( count < MAX_VALUES && mStream.get( digit))
        {
            if( digit == '.')
                digit = '0';

            if( const Traits::Value value = digitToValue<Traits::BOARD_SIZE>( digit); value != -1)
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