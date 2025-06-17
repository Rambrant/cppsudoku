//
// Created by Thomas Rambrant on 2025-06-02.
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
