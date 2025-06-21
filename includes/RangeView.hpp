//
// Created by Thomas Rambrant on 2025-05-16.
//
#pragma once

#include <iterator>
#include "SudokuTraits.hpp"

namespace com::rambrant::sudoku
{
    /** @brief Creates a view (non-owning) into a subrange of a container.
     *
     * @tparam Container A standard container with random-access iterators.
     * @param container The container to view a subrange of.
     * @param from The starting index (inclusive).
     * @param to The ending index (exclusive).
     * @return A lightweight range object that can be used in range-based for-loops.
     *
     * @code
     * std::vector<int> vec{ 1, 2, 3, 4, 5, 6};
     * for( auto val : RangeView( vec, 2, 5))
     * {
     *     std::cout << val << " "; // prints: 3 4 5
     * }
     * @endcode
     */
    template <typename Container>
    class RangeView
    {
        public:
            using Iterator = std::conditional_t< std::is_const_v<Container>,
                typename Container::const_iterator,
                typename Container::iterator>;

            constexpr RangeView( Container& container, std::size_t from, std::size_t to);

            constexpr Iterator begin() const { return mBegin; }
            constexpr Iterator end()   const { return mEnd;   }

        private:

            Iterator mBegin;
            Iterator mEnd;
    };

    //
    // Deduction guide for both const and non-const containers
    //
    template< typename Container>
    RangeView( Container&, std::size_t, std::size_t) -> RangeView<Container>;

    /// \cond DOXYGEN_SUPPRESS

    template <typename Container>
    constexpr RangeView< Container>::RangeView( Container& container, std::size_t from, std::size_t to)
    {
        static_assert( is_random_access_container<Container>, "RangeView requires random-access iterators");

        mBegin = std::next(container.begin(), from);
        mEnd   = std::next(container.begin(), to);
    }

    /// \endcond
}
