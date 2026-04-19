//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include "ICommand.hpp"

namespace com::rambrant::sudoku::gui
{
    /**
     * @brief Resets the entire board to empty and returns to @ref SudokuViewModel::State::Idle.
     */
    class ClearCommand final : public ICommand
    {
        public:
            void execute( SudokuViewModel& viewModel) override;
    };
}