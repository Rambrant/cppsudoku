//
//  Created by Thomas Rambrant on 2025-04-07.
//

#include <regex>
#include <tuple>
#include <utility>

#include "SudokuBoard.hpp"
#include "ISudokuReader.hpp"
#include "ISudokuWriter.hpp"
#include "ISudokuSolver.hpp"
#include "Logger.hpp"

namespace com::rambrant::sudoku
{
    SudokuBoard::SudokuBoard( const ISudokuReader& reader, const ISudokuWriter& writer, SolverList solvers, const Logger& logger) :
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

        int count{1};

        for( const auto& solver : mSolvers)
        {
            mLogger << Logger::verbose << "...Trying solver " << count++ << " --> ";

            Traits::Board       board{ mBoard};
            auto [ result, recursions] = solver->solve( board);

            mLogger << Logger::verbose << "result: " << std::boolalpha << result << ", recursions: " << recursions << std::endl;

            if( result)
            {
                mBoard = board;

                return result;
            }
        }

        return false;
    }
}
