//
//  main.cpp
//
//  Created by Thomas Rambrant on 2025-04-07.
//

#include <fstream>
#include <iostream>

#include "BackTrackingSolver.hpp"
#include "CommandLineParser.h"
#include "ConstraintPropagationSolver.hpp"
#include "FileStream.hpp"
#include "Logger.hpp"
#include "SudokuAsciiReader.hpp"
#include "SudokuBlockWriter.hpp"
#include "SudokuBoard.hpp"
#include "SudokuLineWriter.hpp"
#include "SudokuPrettyWriter.hpp"
#include "SudokuUtil.hpp"

int main( int argc, char* argv[])
{
    using namespace com::rambrant::sudoku;

    try
    {
        using Traits = SudokuTraits;

        //
        // Parse the command line
        //
        BoolOption   verboseOpt{   "--verbose", "-v", false};
        BoolOption   quietOpt{     "--quiet", "-q", false};
        StringOption inputOpt{     "--input", "-i"};
        StringOption outputOpt{    "--output", "-o"};
        StringOption outFormatOpt{ "--output-format", "-O", "block"};
        StringOption inFormatOpt{  "--input-format", "-I", "text"};
        ListOption   solversOpt{   "--solvers", "-s", std::vector<std::string>{ "backtracking", "constraint"}};

        CommandLineParser parser( verboseOpt, quietOpt, inputOpt, outputOpt, outFormatOpt, inFormatOpt, solversOpt);
        parser.parse( argc, argv);

        //
        //  Set the log level
        //
        CommandLineParser::assertNotBoth( verboseOpt, quietOpt);

        auto logLevel{ verboseOpt.isSet() ? Logger::LogLevel::Verbose : quietOpt.isSet() ? Logger::LogLevel::Quiet : Logger::LogLevel::Normal};
        auto logger = Logger{ logLevel };

        logger << Logger::verbose << "Initializing" << std::endl;

        //
        // Initialize the board reader
        //
        CommandLineParser::assertValueIn( inFormatOpt, { "text"});

        std::unique_ptr<ISudokuReader>  reader;
        std::unique_ptr<FileStream>     inputStream;
        std::istream*                   input = &std::cin;

        if( inputOpt.isSet())
        {
            inputStream = std::make_unique<FileStream>( inputOpt.get());
            input       = inputStream.get();

            logger << Logger::verbose << "...Reading from file '" << inputOpt.get() << "'";
        }
        else
        {
            logger << Logger::verbose << "...Reading from stdin";
        }

        if( inFormatOpt.get() == "text")
        {
            reader = std::make_unique<SudokuAsciiReader>( *input, logger);

            logger << Logger::verbose << " [text format]" << std::endl;
        }

        //
        // Initialize the board writer
        //
        CommandLineParser::assertValueIn( outFormatOpt, { "pretty", "block", "line"});

        std::unique_ptr<ISudokuWriter>  writer;
        std::unique_ptr<FileStream>     outputStream;
        std::ostream*                   output = &std::cout;

        if( outputOpt.isSet())
        {
            outputStream = std::make_unique<FileStream>( outputOpt.get(), FileStream::Mode::Write);
            output       = outputStream.get();

            logger << Logger::verbose << "...Writing to file '" << outputOpt.get() << "'";
        }
        else
        {
            logger << Logger::verbose << "...Writing to stdout" ;
        }

        writer = std::make_unique<SudokuBlockWriter>( *output, logger);

        if( outFormatOpt.get() == "pretty")
        {
            writer = std::make_unique<SudokuPrettyWriter>( *output, logger);

            logger << Logger::verbose << " [pretty format]" << std::endl;
        }
        else if( outFormatOpt.get() == "line")
        {
            writer = std::make_unique<SudokuLineWriter>( *output, logger);

            logger << Logger::verbose << " [line format]" << std::endl;
        }
        else
        {
            logger << Logger::verbose << " [block format]" << std::endl;
        }

        //
        // Initialize the solvers
        //
        CommandLineParser::assertValueIn( solversOpt, { "backtracking", "constraint"});

        std::size_t             count{};
        SudokuBoard::SolverList solvers;

        for( const auto& solverArg : solversOpt.get())
        {
            ++count;

            if( solverArg == "backtracking")
            {
                solvers.push_back( std::move( std::make_unique<BackTrackingSolver>( logger)));

                logger << Logger::verbose << "...Adding solver " << count << " [backtracking]" << std::endl;
            }
            else
            {
                solvers.push_back( std::move( std::make_unique<ConstraintPropagationSolver>( logger)));

                logger << Logger::verbose << "...Adding solver " << count << " [constraint propagation]" << std::endl;
            }
        }

        //
        // Setting up and solving the board
        //
        SudokuBoard  board{ *reader, *writer, std::move(solvers), logger};

        logger << Logger::verbose << "Reading [" << Traits::BOARD_SIZE << "x" << Traits::BOARD_SIZE <<"] board" << std::endl;

        board.read();

        if( logger.isVerbose())
        {
            std::cout << std::endl;
            board.write();
            std::cout << std::endl;
        }

        auto  [result, duration] = timedCall([&] { return board.solve(); });

        //
        // Present the result
        //
        if( result)
        {
            logger << "Solved in " << duration.count() << " µs" << std::endl << std::endl;

            board.write();
        }
        else
        {
            throw std::runtime_error( "No solution found!");
        }

        return 0;
    }
    catch( const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 1;
}

