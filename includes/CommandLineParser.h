//
// Created by Thomas Rambrant on 2025-05-13.
//
#pragma once

#include <set>
#include <sstream>
#include <vector>
#include <stdexcept>

#include "Option.hpp"

namespace com::rambrant::sudoku
{
    // Parser class
    class CommandLineParser
    {
        public:

            template< typename... Opts>
            explicit CommandLineParser( Opts&... opts);

            void parse( int argc, char* argv[]) const;

            template< typename T>
            auto get( const Option<T> & opt ) const -> T;

            template< typename T>
            auto assertNotBoth( const Option<T> & lhs, const Option<T> & rhs ) const -> void;

            template<typename A, typename B>
            auto assertRequires( const Option<A> & requiredIfSet, const Option<B> & mustAlsoBeSet ) const -> void;

            template< typename T>
            auto assertValueIn( const Option<T> & opt, const std::set<T> & allowed ) const -> void;

        private:

        std::vector< IOption*> options;
    };


    /// \cond DOXYGEN_SUPPRESS

    template< typename ... Opts>
    CommandLineParser::CommandLineParser( Opts &... opts )
    {
        (options.push_back( &opts), ...);
    }

    template< typename T>
    auto CommandLineParser::get( const Option<T> & opt ) const -> T
    {
        return opt.get();
    }

    template< typename T>
    auto CommandLineParser::assertNotBoth(
        const Option<T> & lhs,
        const Option<T> & rhs ) const -> void
    {
        if( lhs.get() && rhs.get())
        {
            throw std::runtime_error( "Cannot combine " + lhs.getLongFlag() + " and " + rhs.getLongFlag() + "!");
        }
    }

    template< typename A, typename B>
    auto CommandLineParser::assertRequires(
        const Option<A> & requiredIfSet,
        const Option<B> & mustAlsoBeSet ) const -> void
    {
        if( requiredIfSet.isSet() && !mustAlsoBeSet.isSet())
        {
            throw std::runtime_error( requiredIfSet.getLongFlag() + " requires " + mustAlsoBeSet.getLongFlag() + " to be set.");
        }
    }

    template<typename T>
    auto CommandLineParser::assertValueIn(
        const Option<T> &   opt,
        const std::set<T> & allowed ) const -> void
    {
        if( opt.isSet() && allowed.find( opt.get()) == allowed.end())
        {
            std::ostringstream oss;

            oss << "Invalid value for " << opt.getLongFlag() << ": '" << opt.get() << "'. Allowed values are: ";

            int count = 0;
            for( const auto& val : allowed)
            {
                oss << (count++ != 0 ? "," : "") << "'" << val << "'";
            }

            throw std::runtime_error( oss.str());
        }
    }

    /// \endcond
}
