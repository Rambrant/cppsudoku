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