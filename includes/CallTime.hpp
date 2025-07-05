//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <chrono>

namespace com::rambrant::sudoku
{
    /**
     * @brief A "decorator" that measures the execution time for the given function call
     * @param func A callable object to be timed.
     * @param args Arguments to be passed to the callable object.
     * @return A tuple containing the result of the callable object and the execution time.
     *
     * @code
     * auto [result, duration] = timedCall( [&] {return board.solve( solver); });
     * @endcode
     */
    template <typename Func, typename... Args>
    auto CallTime( Func&& func, Args&&... args)
    {
        using ResultType         = std::invoke_result_t< Func, Args...>;
        using DurationResolution = std::chrono::microseconds;

        const auto start = std::chrono::high_resolution_clock::now();

        if constexpr( std::is_void_v< ResultType>)
        {
            std::invoke( std::forward<Func>( func), std::forward<Args>( args)...);

            const auto end      = std::chrono::high_resolution_clock::now();
            const auto duration = std::chrono::duration_cast< DurationResolution>(  end - start);

            return std::make_tuple( std::monostate{}, duration);
        }
        else
        {
            const auto result = std::invoke( std::forward<Func>( func), std::forward<Args>( args)...);

            const auto end      = std::chrono::high_resolution_clock::now();
            const auto duration = std::chrono::duration_cast< DurationResolution>(  end - start);

            return std::make_tuple( result, duration);
        }
    }
}

