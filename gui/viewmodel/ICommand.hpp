//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

namespace com::rambrant::sudoku::gui
{
    class SudokuViewModel;

    /**
     * @brief Interface for all commands that operate on the @ref SudokuViewModel.
     *
     * Commands are the only way state changes propagate into the ViewModel.
     * This keeps all GUI logic testable without any ImGui dependency.
     */
    class ICommand
    {
        public:
            virtual void execute( SudokuViewModel& viewModel) = 0;
            virtual ~ICommand() = default;
    };
}