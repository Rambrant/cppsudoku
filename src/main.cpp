//
//  main.cpp
//
//  Created by Thomas Rambrant on 2025-04-07.
//

#include <iostream>
#include <chrono>

#include "SudokuBoard.hpp"
#include "BackTrackingSolver.hpp"
#include "SudokuStaticReader.hpp"
#include "SudokuFileReader.hpp"
#include "SudokuUtil.hpp"

constexpr SudokuBoard::Traits::Board staticBoard {{
        {{ 8,0,0,  0,0,0, 0,0,0 }},
        {{ 0,0,3,  6,0,0, 0,0,0 }},
        {{ 0,7,0,  0,9,0, 2,0,0 }},

        {{ 0,5,0,  0,0,7, 0,0,0 }},
        {{ 0,0,0,  0,4,5, 7,0,0 }},
        {{ 0,0,0,  1,0,0, 0,3,0 }},

        {{ 0,0,1,  0,0,0, 0,6,8 }},
        {{ 0,0,8,  5,0,0, 0,1,0 }},
        {{ 0,9,0,  0,0,0, 4,0,0 }},
    }};

//------------------------------------------------------------------------------
//
int main()
{
//    auto reader = SudokuStaticReader{ staticBoard};
//    auto solver = BackTrackingSolver{};
    
    auto reader = SudokuFileReader{ std::string( "board.txt")};
    auto solver = BackTrackingSolver{};

    SudokuBoard  board{ reader};
    
    //
    // Print the original board
    //
    std::cout << board << std::endl;
    
    //
    // Solve the given board. Measure the time it takes to solve it
    //
    auto  [stats, duration]     = timedCall([&] { return board.solve( solver); });
    auto& [ result, recursions] = stats;

    //
    // Present the result
    //
    std::cout << "Made " << recursions << " Recursions in " << duration.count() << " µs" << std::endl;
    
    if( result)
    {
        std::cout << "Found a solution "  << std::endl << std::endl << board << std::endl;
    }
    else
    {
        std::cout << std::endl << "No solution found!" << std::endl;
    }
    
    return 0;
}
