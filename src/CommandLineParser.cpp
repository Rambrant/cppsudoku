//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "CommandLineParser.hpp"

#include <iostream>

namespace com::rambrant::sudoku
{
    bool CommandLineParser::parse( const int argc, char *argv[] ) const
    {
        bool result = true;     // Are all options valid after parse?

        for( int i = 1; i < argc; ++i)
        {
            std::string     arg     = argv[ i];
            ICommandOption* matched = nullptr;

            for( auto* opt : mOptions)
            {
                if( opt->isMatched( arg))
                {
                    matched = opt;
                    break;
                }
            }

            if( ! matched)
            {
                std::cerr << "Unknown option: " << arg << '\n';
                continue;
            }

            if( matched->expectsValue())
            {
                if( i + 1 >= argc)
                    throw std::runtime_error( "Expected value after " + arg);

                matched->convertValue( argv[ ++i]);
            }
            else
            {
                matched->convertValue( "");
            }

            if( ! matched->isValid())
            {
                result = false;
            }
        }

        return result;
    }
}
