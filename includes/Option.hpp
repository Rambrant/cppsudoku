//
// Created by Thomas Rambrant on 2025-05-13.
//
#pragma once
#include <string>

#include "IOption.hpp"

namespace com::rambrant::sudoku
{
    /**
     * @brief A template class that implements the interface @ref IOption and is used by the @ref CommandLineParser
     */
    template< typename T>
    class Option : public IOption
    {
        public:

            /**
             * @brief Construct the actual option with the actual underlying type
             * @param longFlag  The long option name, usually prepended with --
             * @param shortFlag The short version of the longFlag, usually just one letter prepended with a single -
             * @param defaultValue
             */
            Option( std::string longFlag, std::string shortFlag, std::optional<T> defaultValue = std::nullopt) :
                mLongFlag( std::move( longFlag)),
                mShortFlag( std::move( shortFlag)),
                mDefaultValue( std::move( defaultValue))
            {}

            /**
             * @brief Check to see if the option matches any command line argument
             * @param arg The argument to match
             * @return true if the arguments match the options short or long names.
             */
            [[nodiscard]]
            bool isMatched( const std::string& arg) const override;

            /**
             * @brief Takes an argument string and converts it into the template type
             * @param arg The command line argument to convert
             */
            void convertValue( const std::string& arg) override;

            /**
             * @brief Does the Option expect a value?
             * @return true if the option expects a value. False if it is a flag.
             */
            [[nodiscard]]
            bool expectsValue() const override;

            /**
              * @brief Gets the value or default. If none exists, an exception is thrown
              * @return The value  if set and the default value otherwise
              */
            T get() const;

            /**
              * @brief Returns the long option name
              * @return The long option name. Usually the name of the option prefixed with '--'
              */
            [[nodiscard]]
            std::string getLongFlag() const override;

            /**
             * @brief Checks to see if a value has been set
             * @return true if the value has been set
             */
            [[nodiscard]]
            bool isSet() const override;

        private:

            std::string         mLongFlag;
            std::string         mShortFlag;
            std::optional<T>    mValue;
            std::optional<T>    mDefaultValue;
};

    /// \cond DOXYGEN_SUPPRESS

    //
    // Type conversion from string to T
    //
    template< typename T>
    T convert( const std::string& s);

    template<>
    inline bool convert<bool>( const std::string& s)
    {
        return s == "1" || s == "true" || s == "yes";
    }

    template<>
    inline std::string convert<std::string>( const std::string& s)
    {
        return s;
    }

    //
    // Member functions
    //
    template<typename T>
    auto Option<T>::isMatched( const std::string & arg ) const -> bool
    {
        return arg == mLongFlag || arg == mShortFlag;
    }

    template<typename T>
    auto Option<T>::convertValue( const std::string & arg ) -> void
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
    auto Option<T>::expectsValue() const -> bool
    {
        return ! std::is_same_v<T, bool>;
    }

    template<typename T>
    auto Option<T>::get() const -> T
    {
        if( mValue)
            return *mValue;

        if( mDefaultValue)
            return *mDefaultValue;

        throw std::runtime_error( "No value set for option " + mLongFlag);
    }

    template<typename T>
    auto Option<T>::getLongFlag() const -> std::string
    {
        return mLongFlag;
    }

    template<typename T>
    auto Option<T>::isSet() const -> bool
    {
        return mValue.has_value();
    }

    /// \endcond
}

