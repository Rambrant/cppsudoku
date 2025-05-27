//
//  StreamUtil.hpp
//
//  Created by Thomas Rambrant on 2025-04-08.
//
#pragma once

#include "SudokuTraits.hpp"

namespace com::rambrant::sudoku
{
    using Traits = SudokuTraits;

    /**
     * @brief A converter from a @ref SudokuTraits::Digit to the internal @ref SudokuTraits::Value. Values over nine are treated as if hexadecimal
     * @tparam size The size of the board, @ref SudokuTraits::BOARD_SIZE.
     * @param digit The character to convert to the corresponding int.
     * @return Value or -1 if the digit could not be converted.
     *
     * @code
     * int val = charToValue<SudokuTraits::BOARD_SIZE>('7');  // Will return 7
     * @endcode
     */
    template< int size>
    int digitToValue( Traits::Digit digit)
    {
        if( digit == '.')
            digit = '0';

        if( std::isdigit( digit))
            return digit - '0';

        //
        // Don't even compile this if the size of the board is 9 or less
        //
        if constexpr( size > 9)
        {
            char c = static_cast<char>( std::toupper( static_cast<unsigned char>(digit))); // A mouthful to avoid 'narrowing conversion' warnings

            if( c >= 'A' && c < 'A' + (size - 9))
                return 10 + (c - 'A');
        }

        return -1;  // skip or invalid
    }

    /**
     * @brief A specialization of the digitToValue taking a @ref Traits::Digit parameter to allow std::string in input
    * @tparam size The size of the board, @ref SudokuTraits::BOARD_SIZE.
     * @param digit The string to convert to the corresponding int.
     * @return Value or -1 if the string could not be converted, or if it contains more than one character.
     */
    template< int size>
    int digitToValue( const std::string& digit)
    {
        if( digit.size() != 1)
            return -1;

        return digitToValue<size>( digit[0]);
    }

    /**
     * @brief A converter from an internal @ref SudokuTraits::Value to the corresponding @ref SudokuTraits::Digit. Values over nine are treated as if hexadecimal
     * @tparam size The size of the board, @ref SudokuTraits::BOARD_SIZE.
     * @param value The Value to convert to the corresponding int.
     * @return The character or '?' if the integer falls outside the allowed value range
     *
     * @code
     * char ch = valueToChar<SudokuTraits::BOARD_SIZE>(7);  // Will return '7'
     * @endcode
     */
    template< int size>
    char valueToDigit( const Traits::Value value)
    {
        if( value >= 0 && value <= 9)
        {
            return static_cast<char>( '0' + value);
        }

        //
        // Don't even compile this if the size of the board is 9 or less
        //
        if constexpr( size > 9)
        {
            if( value >= 10 && value <= size)
                return static_cast<char>( 'A' + (value - 10));
        }

        return '?'; // fallback for unexpected values
    }
}
