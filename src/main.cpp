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

template<typename Stream>
struct StreamDeleter
{
    bool ownsStream = false;

    void operator()( const Stream* stream) const
    {
        if( ownsStream)
        {
            delete stream;
        }
        // Do nothing if we don't own the stream (e.g., std::cin)
    }
};

auto getInputStream( const StringOption& opt, const Logger& logger)
{
    if( opt.isSet())
    {
        const auto& path = opt.get();

        logger << Logger::verbose << "...Reading from file '" << opt.get() << "'";
        return std::unique_ptr<std::istream, StreamDeleter<std::istream>>( new FileStream(path) , StreamDeleter<std::istream>{ true});
    }

    logger << Logger::verbose << "...Reading from stdin";
    return std::unique_ptr<std::istream, StreamDeleter<std::istream>>( &std::cin, StreamDeleter<std::istream>{ false}); // no-op deleter
}

auto getReader( const StringOption& opt, std::istream& stream, const Logger& logger) -> std::unique_ptr<ISudokuReader>
{
    if( opt.get() == "text")
    {
        logger << Logger::verbose << " [text format]" << std::endl;
        return std::make_unique<SudokuAsciiReader>( stream, logger);
    }

    throw std::invalid_argument( "Unsupported input format: " + opt.get());
}

auto getOutputStream( const StringOption& opt, const Logger& logger)
{
    if( opt.isSet())
    {
        const auto& path = opt.get();

        logger << Logger::verbose << "...Writing to file '" << opt.get() << "'";
        return std::unique_ptr<std::ostream, StreamDeleter<std::ostream>>( new FileStream(path, FileStream::Mode::Write) , StreamDeleter<std::ostream>{ true});
    }

    logger << Logger::verbose << "...Writing to stdout";
    return std::unique_ptr<std::ostream, StreamDeleter<std::ostream>>( &std::cout, StreamDeleter<std::ostream>{ false}); // no-op deleter
}

auto getWriter( const StringOption& opt, std::ostream& stream, const Logger& logger) -> std::unique_ptr<ISudokuWriter>
{
    if( opt.get() == "pretty")
    {
        logger << Logger::verbose << " [pretty format]" << std::endl;
        return std::make_unique<SudokuPrettyWriter>( stream, logger);
    }

    if( opt.get() == "line")
    {
        logger << Logger::verbose << " [line format]" << std::endl;
        return std::make_unique<SudokuLineWriter>( stream, logger);
    }

    logger << Logger::verbose << " [block format]" << std::endl;
    return std::make_unique<SudokuBlockWriter>( stream, logger);
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

        auto inputStream = getInputStream( inputOpt, logger);
        auto reader = getReader( inFormatOpt, *inputStream, logger);

        auto outputStream = getOutputStream( outputOpt, logger);
        auto writer = getWriter( outFormatOpt, *outputStream, logger);


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

