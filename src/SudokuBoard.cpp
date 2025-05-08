//
//  SudokoBoard.hpp
//
//  Created by Thomas Rambrant on 2025-04-07.
//

#include <regex>

#include "SudokuBoard.hpp"

#include <tuple>
#include <tuple>
#include <utility>

#include "ISudokuReader.hpp"
#include "ISudokuSolver.hpp"
#include "SudokuUtil.hpp"


SudokuBoard::SudokuBoard( const ISudokuReader& reader, const ISudokuWriter& writer, SolverList solvers) :
    mBoard{},
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

auto SudokuBoard::solve() const -> Traits::BoardResult
{
    for( auto solver : mSolvers)
    {
        Traits::Board       board{ mBoard};
        auto [ result, recursions] = solver.get().solve( board);

        if( result)
        {
            mBoard = board;

            return { result, recursions };
        }
    }

    return { false, 0};
}