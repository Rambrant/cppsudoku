//
// Created by Thomas Rambrant on 2025-05-16.
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
     * @ingroup Helpers
     *
     * @code
     * auto [result, duration] = timedCall( [&] {return board.solve( solver); });
     * @endcode
     */
    template <typename Func, typename... Args>
    auto CallTime( Func&& func, Args&&... args)
    {
        auto start = std::chrono::high_resolution_clock::now();

        auto result = std::invoke( std::forward<Func>( func), std::forward<Args>( args)...);

        auto end      = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( end - start);

        return std::make_tuple( std::move( result), duration);
    }
}

