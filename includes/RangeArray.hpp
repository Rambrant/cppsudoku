//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <array>

namespace com::rambrant::sudoku
{
    /**
     * @brief Generate a constexpr array of N elements using a start value and a step.
     * @tparam N Number of elements in the array.
     * @tparam T Type of the array elements (defaults to `int`).
     * @param start Starting index of the sequence.
     * @param step Increment step between elements.
     * @return A `std::array<T, N>` containing the generated range.
     * @note This function is evaluated at compile-time and is suitable for `constexpr` initialization.
     *
     * @code
     * static constexpr auto odds  = RangeArray<4>(3, 2);  // => {3, 5, 7, 9}
     * static constexpr auto evens = RangeArray<5>(0, 2);  // => {0, 2, 4, 6, 8}
     * @endcode
     */
    template<std::size_t N, typename T = int>
    constexpr std::array<T, N> RangeArray( T start = 0, T step = 1)
    {
        std::array<T, N> result{};

        for( std::size_t i = 0; i < N; ++i)
            result[i] = static_cast<T>( start + static_cast<T>( i) * step);

        return result;
    }
}