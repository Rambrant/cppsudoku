//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <array>
#include <iterator>
#include <type_traits>
#include <vector>
#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

#include "RangeView.hpp"

using namespace com::rambrant::sudoku;

TEST_CASE( "RangeView: size() and empty()", "[unit]")
{
    std::vector vec = {1, 2, 3, 4, 5, 6};

    SECTION( "non-empty view reports correct size")
    {
        RangeView view( vec, 1, 5);
        REQUIRE( view.size() == 4);
        REQUIRE_FALSE( view.empty());
    }

    SECTION( "zero-length view reports size 0 and empty")
    {
        RangeView view( vec, 3, 3);
        REQUIRE( view.size() == 0);
        REQUIRE( view.empty());
    }

    SECTION( "full-range view size equals container size")
    {
        RangeView view( vec, 0, vec.size());
        REQUIRE( view.size() == vec.size());
    }
}

TEST_CASE( "RangeView: mutation through view modifies underlying container", "[unit]")
{
    std::vector vec = {1, 2, 3, 4, 5};
    RangeView view( vec, 1, 4);   // covers indices 1, 2, 3

    for( auto& x : view)
        x = -1;

    REQUIRE( vec[0] ==  1);   // untouched
    REQUIRE( vec[1] == -1);   // mutated
    REQUIRE( vec[2] == -1);   // mutated
    REQUIRE( vec[3] == -1);   // mutated
    REQUIRE( vec[4] ==  5);   // untouched
}

TEST_CASE( "RangeView: generates correct sequences", "[unit]")
{
    SECTION( "RangeView with std::vector")
    {
        std::vector vec = {1, 2, 3, 4, 5, 6};

        RangeView view( vec, 2, 5);

        std::vector      expected = {3, 4, 5};
        std::vector<int> actual;

        for( auto val : view)
        {
            actual.push_back( val);
        }

        REQUIRE( actual == expected);
    }

    SECTION( "RangeView with const std::vector")
    {
        const std::vector vec = {10, 20, 30, 40};

        RangeView view(vec, 1, 3);

        std::vector expected = {20, 30};
        std::vector actual( view.begin(), view.end());

        REQUIRE( actual == expected);
    }

    SECTION( "RangeView with std::array")
    {
        static constexpr std::array<int, 5> arr = {100, 200, 300, 400, 500};

        constexpr RangeView  view( arr, 0, 3);
        std::vector<int>     actual;

        for (int it : view)
        {
            actual.push_back( it);
        }

        REQUIRE( actual == std::vector{ 100, 200, 300});
    }

    SECTION( "RangeView with const std::array")
    {
        constexpr std::array<int, 4> arr = {1, 4, 9, 16};

        RangeView view(arr, 2, 4);

        std::vector actual(view.begin(), view.end());

        REQUIRE( actual == std::vector<int>{9, 16});
    }
}

TEST_CASE( "RangeView supports empty range")
{
    std::vector vec = {42, 43, 44};

    std::vector<int> collected;

    for( int val : RangeView( vec, 2, 2)) // empty range
    {
        collected.push_back( val);
    }

    REQUIRE( collected.empty());
}

TEST_CASE( "RangeView returns random-access iterators", "[unit]")
{
    std::vector                vec = {1, 2, 3};
    [[maybe_unused]] RangeView view(vec, 0, 3);

    using Iterator = decltype( RangeView(vec, 0, 3).begin());
    using Category = std::iterator_traits<Iterator>::iterator_category;

    STATIC_REQUIRE( std::is_base_of_v<std::random_access_iterator_tag, Category>);
}

TEST_CASE( "RangeView: const correctness", "[unit]")
{
    SECTION( "view over const container yields read-only elements")
    {
        const std::vector vec = {1, 2, 3};
        RangeView view( vec, 0, 3);

        using DerefType = decltype( *view.begin());

        //
        // Dereferencing gives a const reference — elements are not assignable
        //
        STATIC_REQUIRE( std::is_const_v<std::remove_reference_t<DerefType>>);
        STATIC_REQUIRE_FALSE( std::is_assignable_v<DerefType, int>);
    }

    SECTION( "view over mutable container yields writable elements")
    {
        std::vector vec = {1, 2, 3};
        RangeView view( vec, 0, 3);

        using DerefType = decltype( *view.begin());

        // Dereferencing gives a mutable reference — elements are assignable
        STATIC_REQUIRE_FALSE( std::is_const_v<std::remove_reference_t<DerefType>>);
        STATIC_REQUIRE( std::is_assignable_v<DerefType, int>);
    }

    SECTION( "const RangeView over mutable container — constness propagation")
    {
        std::vector vec = {1, 2, 3};
        const RangeView view( vec, 0, 3);

        using DerefType = decltype( *view.begin());

        STATIC_REQUIRE_FALSE( std::is_const_v<std::remove_reference_t<DerefType>>);
    }
}