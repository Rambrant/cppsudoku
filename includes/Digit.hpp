//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <expected>
#include "SudokuTraits.hpp"

namespace com::rambrant::sudoku
{
    using Traits = SudokuTraits;

    /**
     * @brief A converter from a @ref SudokuTraits::Digit to the internal @ref SudokuTraits::Value. Values over nine are treated as if hexadecimal
     * @tparam size The size of the board, @ref SudokuTraits::BOARD_SIZE.
     * @param digit The character to convert to the corresponding int.
     * @return std::expected<Traits::Value, std::string> — error if digit is invalid.
     *
     * @code
     * auto val = digitToValue<SudokuTraits::BOARD_SIZE>('7');  // returns expected holding 7
     * @endcode
     */
    template< int size>
    std::expected<Traits::Value, std::string> digitToValue( Traits::Digit digit)
    {
        if( digit == '.')
        {
            digit = '0';
        }

        if( std::isdigit( digit))
        {
            return digit - '0';
        }

        //
        // Don't even compile this if the size of the board is 9 or less
        //
        if constexpr( size > 9)
        {
            char c = static_cast<char>( std::toupper( static_cast<unsigned char>( digit))); // A mouthful to avoid 'narrowing conversion' warnings

            if( c >= 'A' && c < 'A' + (size - 9))
                return 10 + (c - 'A');
        }

        return std::unexpected( std::string("Invalid digit: ") + digit);
    }

    /**
     * @brief A specialization of the digitToValue taking a @ref Traits::Digit parameter to allow std::string in input
    * @tparam size The size of the board, @ref SudokuTraits::BOARD_SIZE.
     * @param digit The string to convert to the corresponding int.
     * @return std::expected<Traits::Value, std::string> — error if not a single valid character.
     */
    template< int size>
    std::expected<Traits::Value, std::string> digitToValue( const std::string& digit)
    {
        if( digit.size() != 1)
        {
            return std::unexpected( "Invalid digit: expected single character, got '" + digit + "'");
        }

        return digitToValue<size>( digit[0]);
    }

    /**
     * @brief A converter from an internal @ref SudokuTraits::Value to the corresponding @ref SudokuTraits::Digit. Values over nine are treated as if hexadecimal
     * @tparam size The size of the board, @ref SudokuTraits::BOARD_SIZE.
     * @param value The Value to convert to the corresponding int.
     * @return std::expected<Traits::Digit, std::string> — error if value is out of range
     *
     * @code
     * auto ch  = valueToDigit<SudokuTraits::BOARD_SIZE>(7);    // returns expected holding '7'
     * @endcode
     */
    template< int size>
    std::expected<Traits::Digit, std::string> valueToDigit( const Traits::Value value)
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

        return std::unexpected( "Invalid value: " + std::to_string( value));
    }
}
