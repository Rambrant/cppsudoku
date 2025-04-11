//
//  StreamUtil.hpp
//  Sudoko
//
//  Created by Thomas Rambrant on 2025-04-08.
//
#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
#include <type_traits>

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
//  for (auto x : subrangeView( aVector, 2, 5)) { std::cout << x << " "; }
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
// A Range implementation for integers
//
class IntIterator
{
    public:
    
        explicit IntIterator( int value) : mValue(value) {}

        int          operator*() const { return mValue; }
        IntIterator& operator++() { ++mValue; return *this; }
        bool         operator!= (const IntIterator& other) const { return mValue != other.mValue; }

    private:
    
        int mValue;
};

class IntRange
{
    public:
    
        IntRange(int begin, int end) : mBegin( begin), mEnd( end) {}

        IntIterator begin() const { return IntIterator( mBegin); }
        IntIterator end() const { return IntIterator( mEnd); }

    private:
    
        int mBegin;
        int mEnd;
};
