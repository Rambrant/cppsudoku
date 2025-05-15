//
// Created by Thomas Rambrant on 2025-05-14.
//
#include "CommandLineParser.h"

#include <iostream>

namespace com::rambrant::sudoku
{
    void CommandLineParser::parse( const int argc, char *argv[] ) const
    {
        for( int i = 1; i < argc; ++i)
        {
            std::string arg     = argv[ i];
            IOption*    matched = nullptr;

            for( auto* opt : options)
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
        }
    }
}
