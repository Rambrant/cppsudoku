//
// Created by Thomas Rambrant on 2025-05-13.
//
#pragma once

#include <set>
#include <sstream>
#include <vector>
#include <stdexcept>

#include "CommandOption.hpp"

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
             * @tparam Opts One or more @ref CommandOption objects to be registered with the parser.
             * @param opts References to the options. Each must be a subclass of @ref ICommandOption.
             */
            template< typename... Opts>
            explicit CommandLineParser( Opts&... opts);

            /**
             * @brief   Parses the command-line arguments given in the main function.
             * @param argc The number of arguments given to the program.
             * @param argv The arguments given to the program.
             */
            auto parse( int argc, char *argv[] ) const -> bool;

            /**
             * @brief Asserts that there is just one of the options is given
             * @tparam A The left-hand type
             * @tparam B The right-hand type
             * @param lhs The left-hand @ref CommandOption
             * @param rhs The right-hand @ref CommandOption
             */
            template< typename A, typename B>
            static auto assertNotBoth( const CommandOption<A>& lhs, const CommandOption<B>& rhs ) -> void;

            /**
             * @brief Asserts that if the left-hand @ref CommandOption is set, then the right-hand @ref CommandOption must also be set
             * @tparam A The left-hand type
             * @tparam B The right-hand type
             * @param lhs The left-hand @ref CommandOption
             * @param rhs The right-hand @ref CommandOption
             */
            template<typename A, typename B>
            static auto assertRequires( const CommandOption<A>& lhs, const CommandOption<B>& rhs ) -> void;

        private:

        std::vector< ICommandOption*> mOptions;
    };

    /// \cond DOXYGEN_SUPPRESS

    template< typename ... Opts>
    CommandLineParser::CommandLineParser( Opts&... opts)
    {
        (mOptions.push_back( &opts), ...);
    }

    template< typename A, typename B>
    auto CommandLineParser::assertNotBoth(
        const CommandOption<A>& lhs,
        const CommandOption<B>& rhs ) -> void
    {
        if( lhs.isSet() && rhs.isSet())
        {
            throw std::runtime_error( "Cannot combine " + lhs.getLongFlag() + " and " + rhs.getLongFlag() + "!");
        }
    }

    template< typename A, typename B>
    auto CommandLineParser::assertRequires(
        const CommandOption<A>& lhs,
        const CommandOption<B>& rhs ) -> void
    {
        if( lhs.isSet() && ! rhs.isSet())
        {
            throw std::runtime_error( lhs.getLongFlag() + " requires " + rhs.getLongFlag() + " to be set.");
        }
    }

    /// \endcond
}
