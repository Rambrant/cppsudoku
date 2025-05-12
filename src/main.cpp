//
//  main.cpp
//
//  Created by Thomas Rambrant on 2025-04-07.
//

#include <iostream>
#include <chrono>
#include <fstream>

#include "SudokuBoard.hpp"
#include "BackTrackingSolver.hpp"
#include "ConstraintPropagationSolver.h"
#include "FileStream.hpp"
#include "SudokuAsciiReader.hpp"
#include "SudokuBlockWriter.hpp"
#include "SudokuLineWriter.hpp"
#include "SudokuPrettyWriter.hpp"
#include "SudokuUtil.hpp"

int main()
{
    try
    {
        auto input  = FileStream{ "board_simple.txt"};
        auto reader = SudokuAsciiReader{ input};
        auto writer = SudokuPrettyWriter( std::cout);
        //    auto writer = SudokuBlockWriter( std::cout);
        //    auto writer = SudokuLineWriter( std::cout);

        auto backtrackSolver  = BackTrackingSolver{};
        auto constraintSolver = ConstraintPropagationSolver{};

        SudokuBoard  board{ reader, writer, { backtrackSolver, constraintSolver}};

        //
        // Print the original board
        //
        board.read();
        board.write();

        std::cout << std::endl;

        auto  [stats, duration] = timedCall([&] { return board.solve(); });

        auto& [ result, recursions] = stats;

        //
        // Present the result
        //
        std::cout << "Made " << recursions << " Recursions in " << duration.count() << " µs" << std::endl;

        if( result)
        {
            std::cout << "Found a solution "  << std::endl;

            board.write();
        }
        else
        {
            std::cout << std::endl << "No solution found!" << std::endl;
        }

        return 0;
    }
    catch( const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 1;
}
