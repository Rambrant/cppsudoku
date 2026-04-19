//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <string>

#include "core/SudokuTraits.hpp"
#include "ICommand.hpp"

namespace com::rambrant::sudoku::gui
{
    /**
     * @brief Holds all state for the Sudoku GUI and exposes it to the View layer.
     *
     * The ViewModel has no ImGui dependency. All state changes go through
     * @ref execute(), which accepts any @ref ICommand. This makes the logic
     * fully unit-testable without a window or render context.
     */
    class SudokuViewModel
    {
        public:
            using Traits = SudokuTraits;

            /// @brief Solver outcome — drives error display in the View
            enum class State
            {
                Idle,    ///< Initial state, board not yet solved
                Solved,  ///< Solver succeeded
                Failed   ///< Solver could not find a solution
            };

            SudokuViewModel();

            // ------------------------------------------------------------------
            // Board access
            // ------------------------------------------------------------------

            /**
             * @brief Returns the current board value at (row, col).
             * @param row  0-based row index
             * @param col  0-based column index
             */
            [[nodiscard]]
            auto getValue( int row, int col) const -> Traits::Value;

            /**
             * @brief Sets a cell value directly (used by the View on user input).
             * @param row   0-based row index
             * @param col   0-based column index
             * @param value 0 = empty, 1-9 = digit
             */
            auto setValue( int row, int col, Traits::Value value) -> void;

            /**
             * @brief Returns the full board — used by commands that call solvers/writers.
             */
            [[nodiscard]]
            auto getBoard() const -> const Traits::Board&;

            /**
             * @brief Replaces the entire board — used by commands that call solvers/readers.
             */
            auto setBoard( const Traits::Board& board) -> void;

            // ------------------------------------------------------------------
            // State access
            // ------------------------------------------------------------------

            [[nodiscard]] auto getState()        const -> State;
            [[nodiscard]] auto getErrorMessage() const -> const std::string&;

            auto setState(       State state)       -> void;
            auto setErrorMessage( std::string msg)  -> void;

            // ------------------------------------------------------------------
            // Command dispatch — the only way state changes happen
            // ------------------------------------------------------------------

            /**
             * @brief Executes a command against this ViewModel.
             *
             * All state mutations go through here, keeping the flow
             * unidirectional and the ViewModel independently testable.
             */
            auto execute( ICommand& command) -> void;

        private:
            Traits::Board mBoard{};         ///< All zeros = empty board
            State         mState  = State::Idle;
            std::string   mErrorMessage;
    };
}