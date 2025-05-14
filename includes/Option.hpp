//
// Created by Thomas Rambrant on 2025-05-13.
//
#pragma once
#include <string>

#include "IOption.hpp"

namespace com::rambrant::sudoku
{
    // Type conversion from string to T
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

    template< typename T>
    class Option : public IOption
    {
        public:
            std::string         mLongFlag;
            std::string         mShortFlag;
            std::optional<T>    mValue;
            std::optional<T>    mDefaultValue;

            Option( std::string longFlag, std::string shortFlag, std::optional<T> defaultValue = std::nullopt) :
                mLongFlag( std::move( longFlag)),
                mShortFlag( std::move( shortFlag)),
                mDefaultValue( std::move( defaultValue))
            {}

            [[nodiscard]]
            bool is( const std::string& arg) const override;

            void parseValue( const std::string& arg) override;

            [[nodiscard]]
            bool expectsValue() const override;

            T get() const;

            [[nodiscard]]
            std::string getLongFlag() const override;

            [[nodiscard]]
            bool isSet() const override;
    };

    /// \cond DOXYGEN_SUPPRESS

    template<typename T>
    auto Option<T>::is( const std::string & arg ) const -> bool
    {
        return arg == mLongFlag || arg == mShortFlag;
    }

    template<typename T>
    auto Option<T>::parseValue( const std::string & arg ) -> void
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

