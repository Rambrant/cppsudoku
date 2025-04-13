//
//  main.cpp
//  Sudoko
//
//  Created by Thomas Rambrant on 2025-04-07.
//

#include <iostream>
#include <chrono>

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
    
    //
    // Measure the time it takes to solve the Sudoku
    //
    auto start = std::chrono::high_resolution_clock::now();

    bool result = board.solve( solver);
    
    auto end      = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( end - start);
    
    //
    // Present the result
    //
    if( result)
    {
        std::cout << "Found solution in " << duration << std::endl << std::endl << board << std::endl;
    }
    else
    {
        std::cout << std::endl << "No solution found!" << std::endl;
    }
    
    return 0;
}
