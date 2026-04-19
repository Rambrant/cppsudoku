//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "core/BoardValidator.hpp"

#include "core/BoardUnits.hpp"

namespace com::rambrant::sudoku
{
    namespace
    {
        bool isUnitValid( const SudokuTraits::BoardArray& unit)
        {
            std::array<bool, Traits::BOARD_SIZE + 1> seen{};

            for( const auto value : unit)
            {
                if( value == Traits::NO_VALUE)
                {
                    continue;
                }
                if( seen[ value])
                {
                    return false;
                }
                seen[ value] = true;
            }
            return true;
        }
    }

    bool isValidBoard( const SudokuTraits::Board& board)
    {
        for( int row = 0; row < Traits::BOARD_SIZE; ++row)
        {
            if( ! isUnitValid( board[ row]))
            {
                return false;
            }
        }

        for( int col = 0; col < Traits::BOARD_SIZE; ++col)
        {
            if( ! isUnitValid( extractColumn( board, col)))
            {
                return false;
            }
        }

        for( int boxRow = 0; boxRow < Traits::BOX_SIZE; ++boxRow)
        {
            for( int boxCol = 0; boxCol < Traits::BOX_SIZE; ++boxCol)
            {
                if( ! isUnitValid( extractBox( board, boxRow * Traits::BOX_SIZE, boxCol * Traits::BOX_SIZE)))
                {
                    return false;
                }
            }
        }

        return true;
    }
}