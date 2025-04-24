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
#include "SudokuStaticReader.hpp"

constexpr SudokuBoard::Traits::Board staticBoard {{
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

//------------------------------------------------------------------------------
//
int main()
{
    SudokuStaticReader staticReader{ staticBoard};
    BackTrackingSolver backTrackingSolver{};
    
    SudokuBoard        board{ staticReader};
    
    //
    // Print the original board
    //
    std::cout << board << std::endl;
    
    //
    // Solve the given board. Measure the time it takes to solve it
    //
    auto start = std::chrono::high_resolution_clock::now();

    bool result = board.solve( backTrackingSolver);
    
    auto end      = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( end - start);
    
    //
    // Present the result
    //
    if( result)
    {
        std::cout << "Found solution in " << duration.count() << " ms" << std::endl << std::endl << board << std::endl;
    }
    else
    {
        std::cout << std::endl << "No solution found!" << std::endl;
    }
    
    return 0;
}
