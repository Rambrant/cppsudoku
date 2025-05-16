//
// Created by Thomas Rambrant on 2025-05-16.
//
#pragma once

namespace com::rambrant::sudoku
{
    namespace helper
    {
        template <typename Iterator>
        struct IteratorRange
        {
            Iterator mBegin;
            Iterator mEnd;

            Iterator begin() const { return mBegin; }
            Iterator end() const { return mEnd; }
        };

        // A deduction guide to help the compiler figure out the correct template arguments
        template<typename Iterator>
        IteratorRange( Iterator, Iterator) -> IteratorRange<Iterator>;
    }

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
     * for( auto val : RangeView( vec, 2, 5))
     * {
     *     std::cout << val << " "; // prints: 3 4 5
     * }
     * @endcode
     */
    template <typename Container>
    auto RangeView( Container& container, std::size_t from, std::size_t to)
    {
        return helper::IteratorRange{
            std::next( container.begin(), from),
            std::next( container.begin(), to)
        };
    }
}