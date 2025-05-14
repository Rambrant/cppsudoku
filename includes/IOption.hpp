//
// Created by Thomas Rambrant on 2025-05-14.
//
#pragma once

#include <string>

namespace com::rambrant::sudoku
{
    class IOption
    {
        public:

            virtual ~IOption() = default;

            [[nodiscard]]
            virtual bool isSet() const = 0;

            [[nodiscard]]
            virtual bool is( const std::string& arg) const = 0;

            virtual void parseValue( const std::string& arg) = 0;

            [[nodiscard]]
            virtual bool expectsValue() const = 0;

            [[nodiscard]]
            virtual std::string getLongFlag() const = 0;
    };
}
