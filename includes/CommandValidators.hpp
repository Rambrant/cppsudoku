//
// Created by Thomas Rambrant on 2025-05-19.
//
#pragma once

#include <iostream>
#include <vector>

class CommandOption;

namespace com::rambrant::sudoku
{
    //
    // Private implementation details
    //
    namespace detail
    {
        template<typename T>
        class ValuesInImpl
        {
            public:
                ValuesInImpl( std::initializer_list<T> allowed);
                explicit ValuesInImpl( std::vector<T> allowed);

                bool operator()(const CommandOption<T>& self) const;
                bool operator()(const CommandOption<std::vector<T>>& self) const;

            private:

                std::vector<T> mAllowed; // Typical small set of values so vector should be faster
        };

        template<typename T>
        class NotWithImpl {
            public:
                explicit NotWithImpl( const CommandOption<T>& other);

                auto operator()( const CommandOption<T>& self) const -> bool;

            private:
                const CommandOption<T>& mOther;
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
        bool ValuesInImpl<T>::operator()( const CommandOption<T> & self) const
        {
            if( ! self.isSet())
                return true;

            const auto& value = self.get();

            if( std::find( mAllowed.begin(), mAllowed.end(), value) == mAllowed.end())
            {
                std::cerr << "Invalid value '" << value << "' for option " << self.getLongFlag() << ". Allowed values: ";

                for( const auto& val : mAllowed)
                    std::cerr << val << " ";

                return false;
            }

            return true;
        }

        template<typename T>
        bool ValuesInImpl<T>::operator()( const CommandOption<std::vector<T>> & self ) const
        {
            if( ! self.isSet())
                return true;

            const auto& value = self.get();

            for( const auto& item : value)
            {
                if( std::find( mAllowed.begin(), mAllowed.end(), item) == mAllowed.end())
                {
                    std::cerr << "Invalid value '" << item << "' for option " << self.getLongFlag() << ". Allowed values: ";

                    for( const auto& val : mAllowed)
                        std::cerr << val << " ";

                    return false;
                }
            }

            return true;
        }

        template<typename T>
        NotWithImpl<T>::NotWithImpl( const CommandOption<T> & other ) :
            mOther( other)
        {}

        template<typename T>
        auto NotWithImpl<T>::operator()( const CommandOption<T>& self) const -> bool
        {
            if( self.isSet() && mOther.isSet())
            {
                std::cerr << "Option " << self.getLongFlag() << " may not be used together with " << mOther.getLongFlag() << std::endl;
                return false;
            }

            return true;
        }
    }

    //
    // Publicly available functions
    //
    template< typename T>
    auto ValuesIn( std::initializer_list<T> allowed)
    {
        return detail::ValuesInImpl<T>(std::vector<T>(allowed.begin(), allowed.end()));
    }

    inline auto ValuesIn( const std::initializer_list<const char*> allowed)
    {
        std::vector<std::string> values;
        values.reserve(allowed.size());

        for( auto s : allowed)
            values.emplace_back(s);

        return detail::ValuesInImpl( std::move(values));
    }

    template< typename T>
    auto NotWith( const CommandOption<T>& other)
    {
        return detail::NotWithImpl<T>( other);
    }
}
