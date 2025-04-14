//
//  StreamUtil.hpp
//
//  Created by Thomas Rambrant on 2025-04-08.
//
#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
#include <type_traits>
#include <utility>


//
// Joins the elements in an Iterable adding the given delimiter between them.
// The result is a string
//
// Usage:
// for( const auto& element : aVector) { std::cout << join( element, ' ') << std::endl; }
//
template< typename Iterable, typename DelimT = std::string>
std::string join( const Iterable& iterable, const DelimT& delimiter)
{
    std::ostringstream os;
    
    auto it  = std::begin( iterable);
    auto end = std::end( iterable);
    
    //
    // Add the first element if the given Iterable is non empty
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

//
// Creates a view into the given container using a range iterator
//
// Usage:
// for (auto x : subrangeView( aVector, 2, 5)) { std::cout << x << " "; }
//
template <typename Iterator>
struct IteratorRange {
    Iterator mBegin;
    Iterator mEnd;
    Iterator begin() const { return mBegin; }
    Iterator end() const { return mEnd; }
};

template <typename Container>
auto subrangeView( Container& c, std::size_t from, std::size_t to)
{
    return IteratorRange{
        std::next( c.begin(), from),
        std::next( c.begin(), to)
    };
}

//
// A Range implementation for constexpr Array initialisation
//
// Usage:
// static constexpr auto VALUE_RANGE = RangeArray<4>(3, 2); // Create the range { 3, 5, 7, 9 }
//

template<std::size_t N, typename T = int, std::size_t... Is>
constexpr std::array<T, N> RangeArrayImpl(T start, T step, std::index_sequence<Is...>)
{
    std::array<T, N> arr{};

    //
    // Use initializer list expansion to assign each element
    //
    ( ( arr[Is] = start + static_cast<T>(Is) * step), ...);

    return arr;
}

template<std::size_t N, typename T = int>
constexpr std::array<T, N> RangeArray(T start = 0, T step = 1)
{
    return RangeArrayImpl<N, T>( start, step, std::make_index_sequence<N>{});
}
