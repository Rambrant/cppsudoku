//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "SudokuBoard.hpp"

#include <future>
#include <regex>
#include <tuple>
#include <thread>
#include <utility>

#include "IReader.hpp"
#include "ISolver.hpp"
#include "IWriter.hpp"
#include "Logger.hpp"

namespace com::rambrant::sudoku
{
    SudokuBoard::SudokuBoard( const IReader& reader, const IWriter& writer, SolverList solvers, const Logger& logger) :
        mBoard{},
        mLogger{ logger},
        mReader{ reader},
        mWriter{ writer},
        mSolvers{ std::move( solvers) }
    {}

    auto SudokuBoard::read() const -> void
    {
        mBoard = mReader.read();
    }

    auto SudokuBoard::write() const -> void
    {
        mWriter.write( mBoard);
    }

    auto SudokuBoard::solve() const -> bool
    {
        mLogger << "Solving Sudoku board" << std::endl;

        int         count{1};
        std::atomic cancelFlag{ false};
        std::vector< std::future< Traits::BoardResult>> futures;

        for( const auto& solver : mSolvers)
        {
            mLogger << Logger::verbose << "...Adding asynchronous solver " << count++ << std::endl;

            futures.emplace_back( std::async( std::launch::async, [ &solver, board = mBoard, &cancelFlag]() mutable -> Traits::BoardResult {
                return solver->solve( board, cancelFlag);
            }));
        }

        //
        // Wait for the first solver to solve the board
        //
        count = 1;  // reset the count

        for( auto& future : futures)
        {
            auto [result, recursions, board] = future.get();

            if( result)
            {
                mLogger << Logger::verbose << "Solver " << count << " solved the board in " << recursions << " recursions" << std::endl;

                mBoard = board;

                return result;
            }

            ++count;
        }

        return false; // None of the solvers solved the board
    }
}
