//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

namespace com::rambrant::sudoku
{
    /**
     * @brief Computes integer square root of a perfect square at compile time using the Newton-Raphson method.
     */
    constexpr auto ConstSqrt( const int number) -> int
    {
        int currentEstimate = number;
        int nextEstimate    = ( currentEstimate + 1) / 2;

        while( nextEstimate < currentEstimate)
        {
            currentEstimate = nextEstimate;
            nextEstimate    = ( currentEstimate + number / currentEstimate) / 2;
        }

        return currentEstimate;
    }
}
