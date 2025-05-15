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
    /**
     * @brief A command-line parser that parses a list of options from the arguments given to the program.
     */
    class CommandLineParser
    {
        public:

            /**
             * @brief Constructs a command-line parser with a list of options.
             *
             * @tparam Opts One or more @ref Option objects to be registered with the parser.
             * @param opts References to the options. Each must be a subclass of @ref IOption.
             */
            template< typename... Opts>
            explicit CommandLineParser( Opts&... opts);

            /**
             * @brief   Parses the command-line arguments given in the main function.
             * @param argc The number of arguments given to the program.
             * @param argv The arguments given to the program.
             */
            auto parse( int argc, char *argv[] ) const -> void;

            /**
             * @brief Asserts that there is just one of the options is given
             * @tparam A The left-hand type
             * @tparam B The right-hand type
             * @param lhs The left-hand @ref Option
             * @param rhs The right-hand @ref Option
             */
            template< typename A, typename B>
            static auto assertNotBoth( const Option<A>& lhs, const Option<B>& rhs ) -> void;

            /**
             * @brief Asserts that if the left hand @ref Option is set, then the right-hand @ref Option must also be set
             * @tparam A The left-hand type
             * @tparam B The right-hand type
             * @param lhs The left-hand @ref Option
             * @param rhs The right-hand @ref Option
             */
            template<typename A, typename B>
            static auto assertRequires( const Option<A>& lhs, const Option<B>& rhs ) -> void;

            /**
             * @brief Asserts that the value in the @ref Option matches the given strings
             * @tparam T The type of the @ref Option
             * @param opt The @ref Option object to check
             * @param allowed A brace-initialized list of string values to check against
             */
            template< typename T>
            static auto assertValueIn( const Option<T>& opt, const std::set<std::string>& allowed) -> void;

        private:

        std::vector< IOption*> options;
    };

    /// \cond DOXYGEN_SUPPRESS

    template< typename ... Opts>
    CommandLineParser::CommandLineParser( Opts&... opts)
    {
        (options.push_back( &opts), ...);
    }

    template< typename A, typename B>
    auto CommandLineParser::assertNotBoth(
        const Option<A>& lhs,
        const Option<B>& rhs ) -> void
    {
        if( lhs.isSet() && rhs.isSet())
        {
            throw std::runtime_error( "Cannot combine " + lhs.getLongFlag() + " and " + rhs.getLongFlag() + "!");
        }
    }

    template< typename A, typename B>
    auto CommandLineParser::assertRequires(
        const Option<A>& lhs,
        const Option<B>& rhs ) -> void
    {
        if( lhs.isSet() && ! rhs.isSet())
        {
            throw std::runtime_error( lhs.getLongFlag() + " requires " + rhs.getLongFlag() + " to be set.");
        }
    }

    template<typename T>
    auto CommandLineParser::assertValueIn(
        const Option<T> &            opt,
        const std::set<std::string>& allowed ) -> void
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
