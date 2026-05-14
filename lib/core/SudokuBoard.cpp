//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "SudokuBoard.hpp"

#include <future>
#include <ranges>
#include <regex>
#include <tuple>
#include <thread>
#include <utility>

#include "BoardValidator.hpp"
#include "../readers/IReader.hpp"
#include "../solvers/ISolver.hpp"
#include "../writers/IWriter.hpp"
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

        if( ! isValidBoard( mBoard))
        {
            throw std::invalid_argument( "Board read from input violates Sudoku constraints");
        }
    }

    auto SudokuBoard::write() const -> void
    {
        mWriter.write( mBoard);
    }

    auto SudokuBoard::solve() const -> bool
    {
        mLogger << "Solving Sudoku board" << std::endl;

        std::atomic cancelFlag{ false};
        std::vector< std::future< Traits::BoardResult>> futures;

        for( const auto& solver : mSolvers)
        {
            mLogger << Logger::verbose << "...Launching solver [" << solver->name() << "]" << std::endl;

            futures.emplace_back( std::async( std::launch::async, [&solver, board = mBoard, &cancelFlag]() mutable -> Traits::BoardResult {
                return solver->solve( board, cancelFlag);
            }));
        }

        for( auto&& [solver, future] : std::views::zip( mSolvers, futures))
        {
            auto [result, recursions, board] = future.get();

            if( result)
            {
                mLogger << Logger::verbose
                        << "Solver [" << solver->name() << "] solved the board in "
                        << recursions << " recursions" << std::endl;

                mBoard = board;

                return true;
            }
        }

        return false; // None of the solvers solved the board
    }
}
