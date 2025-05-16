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
#include <utility>

namespace com::rambrant::sudoku
{
    /// @defgroup Helpers Template Helpers
    /// @brief A collection of template helper functions and structs.
    /// @{

    /**
     * @brief A converter from a character to the corresponding integer. Integers over nine are treated as if hexadecimal
     * @tparam size The size of the board, @ref SudokuTraits::BOARD_SIZE.
     * @param ch The character to convert to the corresponding int.
     * @return The integer or -1 if the character could not be converted.
     * @ingroup Helpers
     *
     * @code
     * int val = charToValue<SudokuTraits::BOARD_SIZE>( '7');  // Will return 7
     * @endcode
     */
    template< int size>
    int charToValue( char ch)
    {
        if( std::isdigit( ch))
            return ch - '0';

        //
        // Don't even compile this if the size of the board is 9 or less
        //
        if constexpr( size > 9)
        {
            char c = static_cast<char>( std::toupper( static_cast<unsigned char>(ch))); // A mouthful to avoid 'narrowing conversion' warnings

            if( c >= 'A' && c < 'A' + (size - 9))
                return 10 + (c - 'A');
        }

        return -1;  // skip or invalid
    }

    /**
     * @brief A converter from an integer to the corresponding character. Integers over nine are treated as if hexadecimal
     * @tparam size The size of the board, @ref SudokuTraits::BOARD_SIZE.
     * @param val The character to convert to the corresponding int.
     * @return The character or '?' if the integer falls outside the allowed value range
     * @ingroup Helpers
     *
     * @code
     * char ch = valueToChar<SudokuTraits::BOARD_SIZE>( 7);  // Will return '7'
     * @endcode
     */
    template< int size>
    char valueToChar( const int val)
    {
        if( val >= 0 && val <= 9)
        {
            return static_cast<char>( '0' + val);
        }

        //
        // Don't even compile this if the size of the board is 9 or less
        //
        if constexpr( size > 9)
        {
            if( val >= 10 && val <= size)
                return static_cast<char>( 'A' + (val - 10));
        }

        return '?'; // fallback for unexpected values
    }

    /// @}
}