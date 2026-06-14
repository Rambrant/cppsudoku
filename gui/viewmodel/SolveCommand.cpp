//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <atomic>

#include "core/BoardValidator.hpp"
#include "core/Logger.hpp"
#include "solvers/BackTrackingSolver.hpp"
#include "viewmodel/SudokuViewModel.hpp"
#include "viewmodel/SolveCommand.hpp"

namespace com::rambrant::sudoku::gui
{
    void SolveCommand::execute( SudokuViewModel& viewModel)
    {
        if( ! isValidBoard( viewModel.getBoard()))
        {
            viewModel.setState(        SudokuViewModel::State::Failed);
            viewModel.setErrorMessage( "Invalid puzzle: duplicate value in row, column or box.");

            return;
        }

        const Logger             logger( Logger::LogLevel::Quiet);

        //GUI uses backtracking only — constraint solver not yet wired up
        const BackTrackingSolver solver( logger);

        auto workingBoard = viewModel.getBoard();
        auto cancelFlag   = std::atomic{ false};
        auto result       = solver.solve( workingBoard, cancelFlag);

        if( result.solved)
        {
            viewModel.setBoard( result.board);
            viewModel.setState( SudokuViewModel::State::Solved);
        }
        else
        {
            viewModel.setState(        SudokuViewModel::State::Failed);
            viewModel.setErrorMessage( "No solution found for this puzzle.");
        }
    }
}