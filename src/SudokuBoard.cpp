//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "SudokuBoard.hpp"

#include <regex>
#include <tuple>
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

        for( const auto& solver : mSolvers)
        {
            mLogger << Logger::verbose << "...Trying solver " << count++ << " --> ";

//            Traits::Board       board{ mBoard};
            auto [ result, recursions, board] = solver->solve( mBoard, cancelFlag);

            if( result)
            {
                mBoard = board;

                return result;
            }
        }

        return false;
    }
}
