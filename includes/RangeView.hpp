//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <iterator>
#include "SudokuTraits.hpp"

namespace com::rambrant::sudoku
{
    /** @brief Creates a view (non-owning) into a subrange of a container.
      *
      * @tparam Container A standard container with random-access iterators.
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

            /// @brief The iterator type from the viewed container
            using Iterator = std::conditional_t< std::is_const_v<Container>,
                typename Container::const_iterator,
                typename Container::iterator>;

            /** @brief Creates a view (non-owning) into a subrange of a container.
             *
             * @param container The container to view a subrange of.
             * @param from The starting index (inclusive).
             * @param to The ending index (exclusive).
             */
            constexpr RangeView( Container& container, std::size_t from, std::size_t to);

            /// The iterator for the first viewed element
            constexpr Iterator begin() const { return mBegin; }

            /// The iterator for the element one past the last viewed element
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
