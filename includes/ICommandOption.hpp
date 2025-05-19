//
// Created by Thomas Rambrant on 2025-05-14.
//
#pragma once

#include <string>

namespace com::rambrant::sudoku
{
    /**
     * @brief An interface for the template class @ref Option used by the @ref CommandLineParser
     */
    class ICommandOption
    {
        public:

            using Validator = std::function<bool( const ICommandOption&)>;

            /**
             * @brief Checks to see if a value has been set
             * @return true if the value has been set
             */
            [[nodiscard]]
            virtual bool isSet() const = 0;

            /**
             * @brief Check to see if the option matches any command line argument
             * @param arg The argument to match
             * @return true if the arguments match the options short or long names.
             */
            [[nodiscard]]
            virtual bool isMatched( const std::string& arg) const = 0;

            /**
             * @brief Takes an argument string and converts it into the @ref Option type
             * @param arg The command line argument to convert
             */
            virtual void convertValue( const std::string& arg) = 0;

            /**
             * @brief Does the @ref Option expect a value?
             * @return True if the option expects a value. False if it is a flag.
             */
            [[nodiscard]]
            virtual bool expectsValue() const = 0;

            /**
             * @brief Returns the long option name
             * @return The long option name. Usually the name of the option prefixed with '--'
             */
            [[nodiscard]]
            virtual std::string getLongFlag() const = 0;

            virtual ~ICommandOption() = default;

        private:

            Validator mValidator{};
    };
}
