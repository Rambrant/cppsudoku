//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "SudokuViewModel.hpp"

#include "core/Digit.hpp"

namespace com::rambrant::sudoku::gui
{
    SudokuViewModel::SudokuViewModel()
    {
        for( auto& row : mBoard)
        {
            row.fill( Traits::NO_VALUE);
        }
    }

    auto SudokuViewModel::getValue( int row, int col) const -> Traits::Value
    {
        return mBoard[ row][ col];
    }

    auto SudokuViewModel::setValue( int row, int col, Traits::Value value) -> void
    {
        mBoard[ row][ col] = value;
        mState             = State::Idle;

        mErrorMessage.clear();
    }

    auto SudokuViewModel::getBoard() const -> const Traits::Board&
    {
        return mBoard;
    }

    auto SudokuViewModel::setBoard( const Traits::Board& board) -> void
    {
        mBoard = board;
    }

    auto SudokuViewModel::getState() const -> State
    {
        return mState;
    }

    auto SudokuViewModel::getErrorMessage() const -> const std::string&
    {
        return mErrorMessage;
    }

    auto SudokuViewModel::setState( State state) -> void
    {
        mState = state;
    }

    auto SudokuViewModel::setErrorMessage( std::string msg) -> void
    {
        mErrorMessage = std::move( msg);
    }

    auto SudokuViewModel::execute( ICommand& command) -> void
    {
        command.execute( *this);
    }
}
