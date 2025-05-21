//
// Created by Thomas Rambrant on 2025-05-19.
//
#pragma once

#include <iostream>
#include <vector>

#include "CommandOption.hpp"

namespace com::rambrant::sudoku
{
    template<typename T>
    class ValuesInImpl
    {
        public:
            ValuesInImpl( std::initializer_list<T> allowed);
            explicit ValuesInImpl( std::vector<T> allowed);

            bool operator()(const CommandOption<T>& option) const;
            bool operator()(const CommandOption<std::vector<T>>& option) const;

        private:

            std::vector<T> mAllowed; // Typical small set of values so vector should be faster
    };

    template<typename T>
    ValuesInImpl<T>::ValuesInImpl( std::initializer_list<T> allowed ) :
        mAllowed( allowed.begin(), allowed.end())
    {}

    template<typename T>
    ValuesInImpl<T>::ValuesInImpl( std::vector<T> allowed ) :
        mAllowed( std::move(allowed))
    {}

    template<typename T>
    bool ValuesInImpl<T>::operator()( const CommandOption<T> & option) const
    {
        if( ! option.isSet())
            return true;

        const auto& value = option.get();

        if( std::find( mAllowed.begin(), mAllowed.end(), value) == mAllowed.end())
        {
            std::cerr << "Invalid value '" << value << "' for option " << option.getLongFlag() << ". Allowed values: ";

            for( const auto& val : mAllowed)
                std::cerr << val << " ";

            return false;
        }

        return true;
    }

    template<typename T>
    bool ValuesInImpl<T>::operator()( const CommandOption<std::vector<T>> & option ) const
    {
        if( ! option.isSet())
            return true;

        const auto& value = option.get();

        for( const auto& item : value)
        {
            if( std::find( mAllowed.begin(), mAllowed.end(), item) == mAllowed.end())
            {
                std::cerr << "Invalid value '" << item << "' for option " << option.getLongFlag() << ". Allowed values: ";

                for( const auto& val : mAllowed)
                    std::cerr << val << " ";

                return false;
            }
        }

        return true;
    }

    template<typename T>
    auto ValuesIn(std::initializer_list<T> allowed)
    {
        return ValuesInImpl<T>(std::vector<T>(allowed.begin(), allowed.end()));
    }

    inline auto ValuesIn( const std::initializer_list<const char*> allowed)
    {
        std::vector<std::string> values;
        values.reserve(allowed.size());

        for( auto s : allowed)
            values.emplace_back(s);

        return ValuesInImpl(std::move(values));
    }
}
