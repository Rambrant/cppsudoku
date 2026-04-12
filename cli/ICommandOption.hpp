//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <string>

namespace com::rambrant::sudoku
{
    /**
     * @brief An interface for the template class @ref CommandOption used by the @ref CommandLineParser
     */
    class ICommandOption
    {
        public:

            /**
             * @brief Checks to see if a value has been set
             * @return true if the value has been set
             */
            [[nodiscard]]
            virtual auto isSet() const -> bool = 0;

            /**
             * @brief Check to see if the option matches any command line argument
             * @param arg The argument to match
             * @return true if the arguments match the options short or long names.
             */
            [[nodiscard]]
            virtual auto isMatched( const std::string & arg ) const -> bool = 0;

            /**
             * @brief Check to see if the option is valid according to the validator function.
             * @return true if the option is valid or if there is no validator added, false if not.
             */
            [[nodiscard]]
            virtual auto isValid() const -> bool = 0;

            /**
             * @brief Takes an argument string and converts it into the @ref CommandOption type
             * @param arg The command line argument to convert
             */
            virtual void convertValue( const std::string& arg) = 0;

            /**
             * @brief Does the @ref CommandOption expect a value?
             * @return True if the option expects a value. False if it is a flag.
             */
            [[nodiscard]]
            virtual bool expectsValue() const = 0;

            /**
             * @brief Returns the long option name
             * @return The long option name. Usually the name of the option is prefixed with '--'
             */
            [[nodiscard]]
            virtual std::string getLongFlag() const = 0;

            virtual ~ICommandOption() = default;
    };
}
