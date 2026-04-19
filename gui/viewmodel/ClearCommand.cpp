//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "core/SudokuTraits.hpp"
#include "SudokuViewModel.hpp"
#include "ClearCommand.hpp"

namespace com::rambrant::sudoku::gui
{
    void ClearCommand::execute( SudokuViewModel& viewModel)
    {
        SudokuTraits::Board empty{};

        for( auto& row : empty)
        {
            row.fill( SudokuTraits::NO_VALUE);
        }

        viewModel.setBoard( empty);
        viewModel.setState( SudokuViewModel::State::Idle);
        viewModel.setErrorMessage( "");
    }
}