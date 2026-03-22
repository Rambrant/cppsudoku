//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <print>
#include <vector>

class CommandOption;

namespace com::rambrant::sudoku
{
    //
    // Private implementation details
    //
    /// \cond INTERNAL
    namespace detail
    {
        template<typename T>
        class ValuesInImpl
        {
            public:
                ValuesInImpl( std::initializer_list<T> allowed, std::ostream& out = std::cout);
                explicit ValuesInImpl( std::vector<T> allowed, std::ostream& out = std::cout);

                bool operator()(const CommandOption<T>& self) const;
                bool operator()(const CommandOption<std::vector<T>>& self) const;

            private:

                std::vector<T> mAllowed; // Typical small set of values so vector should be faster
                std::ostream&  mOut;
        };

        template<typename T>
        class NotWithImpl
        {
            public:
                explicit NotWithImpl( const CommandOption<T>& other, std::ostream& out = std::cout);

                auto operator()( const CommandOption<T>& self) const -> bool;

            private:
                const CommandOption<T>& mOther;
                std::ostream&           mOut;
        };

        template<typename T>
        ValuesInImpl<T>::ValuesInImpl( std::initializer_list<T> allowed, std::ostream& out) :
            mAllowed( allowed.begin(), allowed.end()),
            mOut( out)
        {}

        template<typename T>
        ValuesInImpl<T>::ValuesInImpl( std::vector<T> allowed, std::ostream& out) :
            mAllowed( std::move(allowed)),
            mOut( out)
        {}

        template<typename T>
        bool ValuesInImpl<T>::operator()( const CommandOption<T> & self) const
        {
            if( ! self.isSet())
                return true;

            const auto& value = self.get();

            if( std::find( mAllowed.begin(), mAllowed.end(), value) == mAllowed.end())
            {
                std::print( mOut, "Invalid value '{}' for option {}. Allowed values:", value, self.getLongFlag());

                for( const auto& val : mAllowed)
                    std::print( " {}", val);

                mOut << '\n';

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
                    std::print( mOut, "Invalid value '{}' for option {}. Allowed values:", item, self.getLongFlag());

                    for( const auto& val : mAllowed)
                        std::print( mOut, " {}", val);

                    mOut << '\n';;

                    return false;
                }
            }

            return true;
        }

        template<typename T>
        NotWithImpl<T>::NotWithImpl( const CommandOption<T> & other,  std::ostream& out) :
            mOther( other),
            mOut( out)
        {}

        template<typename T>
        auto NotWithImpl<T>::operator()( const CommandOption<T>& self) const -> bool
        {
            if( self.isSet() && mOther.isSet())
            {
                std::println( mOut, "Option {} may not be used together with {}", self.getLongFlag(), mOther.getLongFlag());
                return false;
            }

            return true;
        }
    }

    /// \endcond

    //
    // Publicly available functions
    //
    /**
     * @brief Validates a @ref CommandOption to ensure that it has a value in a predefined set of values
     *
     * @tparam T The type of the option
     * @param allowed The set of allowed values
     * @return A validator function that can be used with @ref CommandOption::addValidator
     */
    template< typename T>
    auto ValuesIn( std::initializer_list<T> allowed,  std::ostream& out = std::cout)
    {
        return detail::ValuesInImpl<T>( std::vector<T>(allowed.begin(), allowed.end()), out);
    }

    /**
     * @brief Validates a @ref CommandOption to ensure that it has a value in a predefined set of values
     *
     * @tparam T The type of the option
     * @param allowed The set of allowed values
     * @return A validator function that can be used with @ref CommandOption::addValidator
     */
    inline auto ValuesIn( const std::initializer_list<const char*> allowed,  std::ostream& out = std::cout)
    {
        std::vector<std::string> values;
        values.reserve(allowed.size());

        for( auto s : allowed)
            values.emplace_back(s);

        return detail::ValuesInImpl( std::move(values), out);
    }

    /**
     * @brief Validates a @ref CommandOption to ensure that it isn't given togehter with another option
     *
     * @tparam T The type of the option
     * @param other The option to check against
     * @return true if the option isn't given together with other, false otherwise
     */
    template< typename T>
    auto NotWith( const CommandOption<T>& other, std::ostream& out = std::cout)
    {
        return detail::NotWithImpl<T>( other, out);
    }
}
