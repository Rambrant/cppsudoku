//
//  main.cpp
//
//  Created by Thomas Rambrant on 2025-04-07.
//

#include <iostream>
#include <fstream>

#include "SudokuBoard.hpp"
#include "Logger.hpp"
#include "BackTrackingSolver.hpp"
#include "ConstraintPropagationSolver.hpp"
#include "FileStream.hpp"
#include "SudokuAsciiReader.hpp"
#include "SudokuBlockWriter.hpp"
#include "SudokuLineWriter.hpp"
#include "SudokuPrettyWriter.hpp"
#include "SudokuUtil.hpp"

int main()
{
    using namespace com::rambrant::sudoku;

    try
    {
        auto logger = Logger{ Logger::LogLevel::Normal};
        auto input  = FileStream{ "board_hard.txt"};
        auto reader = SudokuAsciiReader{ input, logger};
        auto writer = SudokuPrettyWriter( std::cout, logger);
        //    auto writer = SudokuBlockWriter( std::cout, logger);
        //    auto writer = SudokuLineWriter( std::cout, logger);

        auto backtrackSolver  = BackTrackingSolver{ logger};
        auto constraintSolver = ConstraintPropagationSolver{ logger};

        SudokuBoard  board{ reader, writer, { backtrackSolver, constraintSolver}, logger};

        //
        // Print the original board
        //
        board.read();

        //
        // Solve the board
        //
        auto  [result, duration] = timedCall([&] { return board.solve(); });

        //
        // Present the result
        //
        if( result)
        {
            logger << "Solved in " << duration.count() << " µs" << std::endl << std::endl;

            board.write();
        }
        else
        {
            throw std::runtime_error( "No solution found!");
        }

        return 0;
    }
    catch( const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 1;
}

