//
//  StreamUtil.hpp
//
//  Created by Thomas Rambrant on 2025-04-08.
//
#pragma once

/// @defgroup Helpers Template Helpers
/// @brief A collection of template helper functions and structs.
/// @{

#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
#include <utility>

/**
 * @brief Joins the elements in an Iterable adding the given delimiter between them.
 * @param iterable The container or other iterable object to be joined. eg. a vector, list, set, string etc.
 * @param delimiter The string to delimit the elements with. eg. " " for space, "," for comma, ";" for semicolon, etc.
 * @return The resulting string.
 * @ingroup Helpers
 *
 * @code
 * std::<vector<int> { 1, 2, 3};
 * std::cout << join( element, ', ') << std::endl; // Prints "1, 2, 3"
 * @endcode
 */
template< typename Iterable, typename DelimT = std::string>
std::string join( const Iterable& iterable, const DelimT& delimiter)
{
    std::ostringstream os;
    
    auto it  = std::begin( iterable);
    auto end = std::end( iterable);
    
    //
    // Add the first element if the given Iterable is non-empty
    //
    if( it != end)
    {
        os << *it;
        ++it;
    }
    
    //
    // Add the rest of the elements separated by the delimiter, if any
    //
    for( ; it != end; ++it)
    {
        os << delimiter << *it;
    }
    
    return os.str();
}

/**
 * @cond INTERNAL
 * @brief A lightweight iterator-based view of a subrange in a container.
 *
 * @tparam Iterator The iterator type for the container.
 * @ingroup Helpers
 */
template <typename Iterator>
struct IteratorRange
{
    Iterator mBegin;
    Iterator mEnd;

    Iterator begin() const { return mBegin; }
    Iterator end() const { return mEnd; }
};
/// @endcond


/// @cond DOXYGEN_SHOULD_SKIP_THIS
// A deduction guide to help the compiler figure out the correct template arguments
template <typename Iterator>
IteratorRange(Iterator, Iterator) -> IteratorRange<Iterator>;
/// @endcond

/** @brief Creates a view (non-owning) into a subrange of a container.
 *
 * @tparam Container A standard container with random-access iterators.
 * @param container The container to view a subrange of.
 * @param from The starting index (inclusive).
 * @param to The ending index (exclusive).
 * @return A lightweight range object that can be used in range-based for-loops.
 * @ingroup Helpers
 *
 * @code
 * std::vector<int> vec{ 1, 2, 3, 4, 5, 6};
 * for( auto val : subrangeView( vec, 2, 5))
 * {
 *     std::cout << val << " "; // prints: 3 4 5
 * }
 * @endcode
 */
template <typename Container>
auto subrangeView( Container& container, std::size_t from, std::size_t to)
{
    return IteratorRange{
        std::next( container.begin(), from),
        std::next( container.begin(), to)
    };
}

//------------------------------------------------------------------------------
//
// A Range implementation for constexpr Array initialization
//
// Usage:
// static constexpr auto VALUE_RANGE = RangeArray<4>(3, 2); // Create the range { 3, 5, 7, 9 }
//


/**
 * @cond INTERNAL
 * @brief Internal implementation of `RangeArray` using index sequence expansion.
 *
 * @internal
 * This function should not be called directly. Used by `RangeArray`.
 */
template<std::size_t N, typename T = int, std::size_t... Is>
constexpr std::array<T, N> RangeArrayImpl(T start, T step, std::index_sequence<Is...>)
{
    std::array<T, N> arr{};

    //
    // Use initializer list expansion to assign each element
    //
    (( arr[Is] = start + static_cast<T>(Is) * step), ...);

    return arr;
}
/// @endcond

/**
 * @brief Generate a constexpr array of N elements using a start value and a step.
 * @tparam N Number of elements in the array.
 * @tparam T Type of the array elements (defaults to `int`).
 * @param start Starting index of the sequence.
 * @param step Increment step between elements.
 * @return A `std::array<T, N>` containing the generated range.
 * @note This function is evaluated at compile-time and is suitable for `constexpr` initialization.
 * @see RangeArrayImpl (internal helper)
 * @ingroup Helpers
 *
 * @code
 * static constexpr auto odds = RangeArray<4>(3, 2);  // => {3, 5, 7, 9}
 * static constexpr auto evens = RangeArray<5>(0, 2); // => {0, 2, 4, 6, 8}
 * @endcode
 */
template<std::size_t N, typename T = int>
constexpr std::array<T, N> RangeArray(T start = 0, T step = 1)
{
    return RangeArrayImpl<N, T>( start, step, std::make_index_sequence<N>{});
}

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
auto timedCall(Func&& func, Args&&... args)
{
    auto start = std::chrono::high_resolution_clock::now();

    auto result = std::invoke( std::forward<Func>( func), std::forward<Args>( args)...);

    auto end      = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( end - start);

    return std::make_tuple( std::move( result), duration);
}

/// @}