//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <sstream>
#include <vector>

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

        private:

        std::vector< ICommandOption*> mOptions;
    };

    /// \cond DOXYGEN_SUPPRESS

    template< typename ... Opts>
    CommandLineParser::CommandLineParser( Opts&... opts)
    {
        (mOptions.push_back( &opts), ...);
    }

    /// \endcond
}
