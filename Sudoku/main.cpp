//
//  main.cpp
//  Sudoko
//
//  Created by Thomas Rambrant on 2025-04-07.
//

#include <iostream>

#include "SudokuBoard.hpp"
#include "BackTrackingSolver.hpp"

constexpr SudokuBoard::Traits::Board initBoard {{
        {{8, 0, 0, 0, 0, 0, 0, 0, 0}},
        {{0, 0, 3, 6, 0, 0, 0, 0, 0}},
        {{0, 7, 0, 0, 9, 0, 2, 0, 0}},
        {{0, 5, 0, 0, 0, 7, 0, 0, 0}},
        {{0, 0, 0, 0, 4, 5, 7, 0, 0}},
        {{0, 0, 0, 1, 0, 0, 0, 3, 0}},
        {{0, 0, 1, 0, 0, 0, 0, 6, 8}},
        {{0, 0, 8, 5, 0, 0, 0, 1, 0}},
        {{0, 9, 0, 0, 0, 0, 4, 0, 0}},
    }};

int main()
{
    SudokuBoard        board  = SudokuBoard( initBoard);
    BackTrackingSolver solver = BackTrackingSolver();
    
    std::cout << board << std::endl;
    
    board.solve( solver);
    
    std::cout << std::endl << board << std::endl;
    
    return 0;
}
