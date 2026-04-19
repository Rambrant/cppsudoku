//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "core/BoardUnits.hpp"
#include <algorithm>
#include "core/RangeView.hpp"

namespace com::rambrant::sudoku
{
    using Traits = SudokuTraits;

    auto extractColumn( const Traits::Board& board, int col) -> Traits::BoardArray
    {
        Traits::BoardArray column{};

        std::transform( board.begin(), board.end(), column.begin(),
            [col]( const Traits::BoardArray& row) { return row[ col]; });

        return column;
    }

    auto extractBox( const Traits::Board& board, int row, int col) -> Traits::BoardArray
    {
        const int startRow = (row / Traits::BOX_SIZE) * Traits::BOX_SIZE;
        const int endRow   = startRow + Traits::BOX_SIZE;

        const int startCol = (col / Traits::BOX_SIZE) * Traits::BOX_SIZE;
        const int endCol   = startCol + Traits::BOX_SIZE;

        Traits::BoardArray box{};
        auto boxIter = box.begin();

        for( const auto& boardRow : RangeView( board, startRow, endRow))
        {
            for( const int value : RangeView( boardRow, startCol, endCol))
            {
                *boxIter++ = value;
            }
        }

        return box;
    }
}