//
// Created by Thomas Rambrant on 2025-05-13.
//
#pragma once
#include <functional>
#include <string>
#include <sstream>
#include <vector>

#include "ICommandOption.hpp"

namespace com::rambrant::sudoku
{
    /**
     * @brief A template class that implements the interface @ref ICommandOption and is used by the @ref CommandLineParser
     */
    template< typename T>
    class CommandOption : public ICommandOption
    {
        public:

            /**
             * @brief A type for a callable validator that can be added to an option. See @ref CommandValidators.hpp
             */
            using ValidatorFunc = std::function<bool( const CommandOption<T>&)>;

            /**
             * @brief Construct the option with the given value
             * @param longFlag  The long option name, usually prepended with --
             * @param shortFlag The short version of the longFlag, usually just one letter prepended with a single -
             * @param defaultValue The default value if no value is set
             */
            CommandOption( std::string longFlag, std::string shortFlag, std::optional<T> defaultValue = std::nullopt) :
                mLongFlag( std::move( longFlag)),
                mShortFlag( std::move( shortFlag)),
                mDefaultValue( std::move( defaultValue))
            {}

            /**
             * @brief Construct the option with the given value. This is an alternative version that supports a list of values
             * @param longFlag  The long option name, usually prepended with '--'
             * @param shortFlag The short version of the longFlag, usually just one letter prepended with a single '-'
             * @param defaultValue The default value if no value is set in the form of a bace-initializer or a vector of strings.
             */
            //
            // Using a constraint to ensure that this constructor is only used for List types of options
            //
            template< typename U = T, typename = std::enable_if_t< std::is_same_v<U, std::vector<std::string>> >>
            CommandOption( std::string longFlag, std::string shortFlag, std::vector<std::string> defaultValue = {} ) :
                CommandOption( std::move( longFlag), std::move( shortFlag), std::optional{ std::move( defaultValue)}) {}


            /**
             * @brief Sets the validator function. The validator function is called with the option as an argument.
             */
            void setValidator( ValidatorFunc validator);

            /**
             * @brief Check to see if the option matches any command line argument
             * @param arg The argument to match
             * @return true if the arguments match the options short or long names.
             */
            [[nodiscard]]
            auto isMatched( const std::string & arg ) const -> bool override;

            /**
             * @brief Check to see if the option is valid according to the validator function.
             * @return true if the option is valid or if there is no validator added, false if not.
             */
            [[nodiscard]]
            auto isValid() const -> bool override;

            /**
             * @brief Takes an argument string and converts it into the template type
             * @param arg The command line argument to convert
             */
            auto convertValue( const std::string & arg ) -> void override;

            /**
             * @brief Does the Option expect a value?
             * @return True if the option expects a value. False if it is a flag.
             */
            [[nodiscard]]
            auto expectsValue() const -> bool override;

            /**
              * @brief Gets the value or default. If none exists, an exception is thrown
              * @return The value  if set and the default value otherwise
              */
            [[nodiscard]]
            auto get() const -> T;

            /**
              * @brief Returns the long option name
              * @return The long option name. Usually the name of the option prefixed with '--'
              */
            [[nodiscard]]
            auto getLongFlag() const -> std::string override;

            /**
             * @brief Checks to see if a value has been set
             * @return true if the value has been set
             */
            [[nodiscard]]
            auto isSet() const -> bool override;

        private:

            std::string        mLongFlag;
            std::string        mShortFlag;
            std::optional<T>   mValue;
            std::optional<T>   mDefaultValue;
            ValidatorFunc      mValidator;
};

    /**
     * @brief Convenience class introducing a short notion for Option<bool>
     */
    class BoolOption final : public CommandOption<bool>
    {
        public:
            using CommandOption::CommandOption;
    };

    /**
     * @brief Convenience class introducing a short notion for Option<std::string>
     */
    class StringOption final : public CommandOption<std::string>
    {
        public:
            using CommandOption::CommandOption;
    };

    /**
    * @brief Convenience class introducing a short notion for Option<std::vector<std::string>>
    */
    class ListOption final : public CommandOption<std::vector<std::string>>
    {
        public:
            using CommandOption::CommandOption;
    };


    /// \cond DOXYGEN_SUPPRESS

    //
    // Type conversion from string to T
    //
    template< typename T>
    auto convert( const std::string & arg) -> T;

    template<>
    inline auto convert<bool>( const std::string & arg) -> bool
    {
        return arg == "1" || arg == "true" || arg == "yes";
    }

    template<>
    inline auto convert<std::string>( const std::string & arg) -> std::string
    {
        return arg;
    }

    template<>
    inline auto convert<std::vector<std::string>>( const std::string & arg) -> std::vector<std::string>
    {
        std::vector<std::string> result;
        std::stringstream        stream( arg);
        std::string              token;

        while( std::getline( stream, token, ','))
        {
            result.push_back( token);
        }

        return result;
    }

    template<typename T>
    auto CommandOption<T>::setValidator( ValidatorFunc validator ) -> void
    {
        mValidator = std::move( validator);
    }

    //
    // Member functions
    //
    template<typename T>
    auto CommandOption<T>::isMatched( const std::string & arg ) const -> bool
    {
        return arg == mLongFlag || arg == mShortFlag;
    }

    template<typename T>
    auto CommandOption<T>::isValid() const -> bool
    {
        if( ! mValidator)
            return true;

        return mValidator( *this);
    }

    template<typename T>
    auto CommandOption<T>::convertValue( const std::string & arg ) -> void
    {
        if constexpr( std::is_same_v<T, bool>)
        {
            mValue = true;
        }
        else
        {
            mValue = convert<T>( arg);
        }
    }

    template<typename T>
    auto CommandOption<T>::expectsValue() const -> bool
    {
        return ! std::is_same_v<T, bool>;
    }

    template<typename T>
    auto CommandOption<T>::get() const -> T
    {
        if( mValue)
            return *mValue;

        if( mDefaultValue)
            return *mDefaultValue;

        throw std::runtime_error( "No value set for option " + mLongFlag);
    }

    template<typename T>
    auto CommandOption<T>::getLongFlag() const -> std::string
    {
        return mLongFlag;
    }

    template<typename T>
    auto CommandOption<T>::isSet() const -> bool
    {
        return mValue.has_value();
    }

    /// \endcond
}

