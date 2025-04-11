//
//  Board.cpp
//  Sudoko
//
//  Created by Thomas Rambrant on 2025-04-07.
//

#include "SudokuBoard.hpp"

#include "StreamUtil.hpp"


SudokuBoard::SudokuBoard(std::array<std::array<int, Config::BOARD_SIZE>, Config::BOARD_SIZE> anInitMatrix):
    mBoard( std::move( anInitMatrix))
{
    static_assert (mBoard.size() == Config::BOARD_SIZE);
};

std::ostream& operator<<( std::ostream& os, const SudokuBoard& aBoard)
{
    for( const auto& row : aBoard.mBoard)
    {
        std::cout << join( row, ' ') << std::endl;
    }

    return os;
}
