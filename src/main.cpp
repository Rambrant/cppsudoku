//
//  main.cpp
//
//  Created by Thomas Rambrant on 2025-04-07.
//

#include <fstream>
#include <iostream>

#include "BackTrackingSolver.hpp"
#include "CallTime.hpp"
#include "CommandLineParser.hpp"
#include "CommandValidators.hpp"
#include "ConstraintPropagationSolver.hpp"
#include "FileStream.hpp"
#include "Logger.hpp"
#include "SudokuAsciiReader.hpp"
#include "SudokuBlockWriter.hpp"
#include "SudokuBoard.hpp"
#include "SudokuLineWriter.hpp"
#include "SudokuPrettyWriter.hpp"

using namespace com::rambrant::sudoku;

auto getLogger( const BoolOption& verboseOpt, const BoolOption & quietOpt) -> Logger
{
    auto logLevel{ verboseOpt.isSet() ? Logger::LogLevel::Verbose : quietOpt.isSet() ? Logger::LogLevel::Quiet : Logger::LogLevel::Normal};
    auto logger = Logger{ logLevel};

    logger << Logger::verbose << "Initializing" << std::endl << "...Logger" << std::endl;

    return logger;
}

struct StreamDeleter
{
    bool ownsStream = false;

    void operator()( const std::istream* stream) const
    {
        if( ownsStream)
        {
            delete stream;
        }
        // Do nothing if we don't own the stream (e.g., std::cin)
    }
};

auto getInputStream( const StringOption& inputOpt, const Logger& logger) -> std::unique_ptr<std::istream, StreamDeleter>
{
    if( inputOpt.isSet())
    {
        const auto& path = inputOpt.get();

        logger << Logger::verbose << "...Reading from file '" << inputOpt.get() << "'";
        return std::unique_ptr<std::istream, StreamDeleter>( new FileStream(path) , StreamDeleter{ true});
    }

    logger << Logger::verbose << "...Reading from stdin";
    return std::unique_ptr<std::istream, StreamDeleter>( &std::cin, StreamDeleter{ false}); // no-op deleter
}

auto getReader( const StringOption& inFormatOpt, std::istream& stream, const Logger& logger) -> std::unique_ptr<ISudokuReader>
{
    if (inFormatOpt.get() == "text")
    {
        logger << Logger::verbose << " [text format]" << std::endl;
        return std::make_unique<SudokuAsciiReader>( stream, logger);
    }

    throw std::invalid_argument( "Unsupported input format: " + inFormatOpt.get());
}

int main( int argc, char* argv[])
{

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

        verboseOpt.setValidator( NotWith( quietOpt));
        quietOpt.setValidator( NotWith( verboseOpt));
        outFormatOpt.setValidator( ValuesIn( { "pretty", "block", "line"}));
        inFormatOpt.setValidator( ValuesIn( { "text"}));
        solversOpt.setValidator( ValuesIn( { "backtracking", "constraint"}));

        CommandLineParser parser( verboseOpt, quietOpt, inputOpt, outputOpt, outFormatOpt, inFormatOpt, solversOpt);

        if( ! parser.parse( argc, argv))
            return 1;

        //
        // Set things up
        //
        Logger logger = getLogger( verboseOpt, quietOpt);

        auto stream = getInputStream( inputOpt, logger);
        auto reader = getReader( inFormatOpt, *stream, logger);


        //
        // Initialize the board writer
        //
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

        auto  [result, duration] = CallTime([&] { return board.solve(); });

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

