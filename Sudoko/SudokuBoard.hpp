//
//  Board.hpp
//  Sudoko
//
//  Created by Thomas Rambrant on 2025-04-07.
//
#pragma once

#include <iostream>
#include <vector>

namespace Config
{
    inline constexpr int BOARD_SIZE      = 9;
    inline constexpr int SUBSECTION_SIZE = 3;
    inline constexpr int NO_VALUE        = 0;
}

//
// The board class
//
class SudokuBoard
{
    private:
    
        std::array<std::array<int, Config::BOARD_SIZE>, Config::BOARD_SIZE> mBoard;
        
    public:
    
        SudokuBoard( std::array<std::array<int, Config::BOARD_SIZE>, Config::BOARD_SIZE> anInitMatrix);
        
        friend std::ostream& operator<<(std::ostream& os, const SudokuBoard& board);
};
