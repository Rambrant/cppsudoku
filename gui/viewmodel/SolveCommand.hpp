//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include "ICommand.hpp"

namespace com::rambrant::sudoku::gui
{
    /**
     * @brief Runs the backtracking solver on the current board state.
     *
     * On success the ViewModel board is replaced with the solution and
     * state is set to @ref SudokuViewModel::State::Solved.
     * On failure state is set to @ref SudokuViewModel::State::Failed
     * and an error message is written.
     */
    class SolveCommand final : public ICommand
    {
        public:
            void execute( SudokuViewModel& viewModel) override;
    };
}