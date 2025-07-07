//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <fstream>
#include <iostream>

#include "AsciiReader.hpp"
#include "BackTrackingSolver.hpp"
#include "BlockWriter.hpp"
#include "CallTime.hpp"
#include "CommandLineParser.hpp"
#include "CommandValidators.hpp"
#include "ConstraintPropagationSolver.hpp"
#include "FileStream.hpp"
#include "JsonReader.hpp"
#include "JsonWriter.hpp"
#include "LineWriter.hpp"
#include "Logger.hpp"
#include "PrettyWriter.hpp"
#include "SudokuBoard.hpp"

using namespace com::rambrant::sudoku;
using namespace std::string_literals;

//
// Helper functions
//
/// \cond INTERNAL
auto getLogger( const BoolOption& verboseOpt, const BoolOption & quietOpt) -> Logger
{
    const auto logLevel{ verboseOpt.isSet() ? Logger::LogLevel::Verbose : quietOpt.isSet() ? Logger::LogLevel::Quiet : Logger::LogLevel::Normal};
    const auto logger = Logger{ logLevel};

    logger <<  "Initializing" << std::endl << Logger::verbose << "...Logger" << std::endl;

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

auto getReader( const StringOption& opt, std::istream& stream, const Logger& logger) -> std::unique_ptr<IReader>
{
    if( opt.get() == "json")
    {
        logger << Logger::verbose << " [json format]" << std::endl;
        return std::make_unique<JsonReader>( stream, logger);
    }

    logger << Logger::verbose << " [text format]" << std::endl;
    return std::make_unique<AsciiReader>( stream, logger);
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

auto getWriter( const StringOption& opt, std::ostream& stream, const Logger& logger) -> std::unique_ptr<IWriter>
{
    if( opt.get() == "pretty")
    {
        logger << Logger::verbose << " [pretty format]" << std::endl;
        return std::make_unique<PrettyWriter>( stream, logger);
    }

    if( opt.get() == "line")
    {
        logger << Logger::verbose << " [line format]" << std::endl;
        return std::make_unique<LineWriter>( stream, logger);
    }

    if( opt.get() == "json")
    {
        logger << Logger::verbose << " [json format]" << std::endl;
        return std::make_unique<JsonWriter>( stream, logger);
    }

    logger << Logger::verbose << " [block format]" << std::endl;
    return std::make_unique<BlockWriter>( stream, logger);
}

auto getSolvers( const ListOption& opt, const Logger& logger) -> SudokuBoard::SolverList
{
    std::size_t             count{};
    SudokuBoard::SolverList solvers;

    for( const auto& solverArg : opt.get())
    {
        ++count;

        if( solverArg == "backtracking")
        {
            logger << Logger::verbose << "...Adding solver " << count << " [backtracking]" << std::endl;
            solvers.push_back( std::make_unique<BackTrackingSolver>( logger));
        }
        else
        {
            logger << Logger::verbose << "...Adding solver " << count << " [constraint propagation]" << std::endl;
            solvers.push_back( std::make_unique<ConstraintPropagationSolver>( logger));
        }
    }

    return solvers;
}
/// \endcond

//
// Main function
//
int main( int argc, char* argv[])
{
    try
    {
        using Traits = SudokuTraits;

        //
        // Parse the command line
        //
        auto helpText = "Usage: sudoku_solver [OPTIONS]\n"s
                      + "Options:\n"s
                      + "  -h, --help                 Show this help message and exit\n"s
                      + "  -i, --input <file>         Read puzzle from file. (Default: stdin)\n"s
                      + "  -I, --input-format <fmt>   Output format: text or json. (Default: text)\n"s
                      + "  -o, --output <file>        Write solution to file. (Default: stdout\n"s
                      + "  -O, --output-format <fmt>  Output format: pretty, block, line or json. (Default: block)\n"s
                      + "  -s, --solvers <solver,...> Solvers to use: backtracking or constraint. (Default: backtracking and constraint)\n"s
                      + "  -v, --verbose              Verbose output\n"s
                      + "  -q, --quiet                Quiet output\n"s;

        BoolOption   helpOpt{      "--help", "-h", false};
        BoolOption   verboseOpt{   "--verbose", "-v", false};
        BoolOption   quietOpt{     "--quiet", "-q", false};
        StringOption inputOpt{     "--input", "-i"};
        StringOption outputOpt{    "--output", "-o"};
        StringOption outFormatOpt{ "--output-format", "-O", "block"};
        StringOption inFormatOpt{  "--input-format", "-I", "text"};
        ListOption   solversOpt{   "--solvers", "-s", { "backtracking", "constraint"}};

        verboseOpt.setValidator( NotWith( quietOpt));
        quietOpt.setValidator( NotWith( verboseOpt));
        outFormatOpt.setValidator( ValuesIn( { "pretty", "block", "line", "json"}));
        inFormatOpt.setValidator( ValuesIn( { "text", "json"}));
        solversOpt.setValidator( ValuesIn( { "backtracking", "constraint"}));

        CommandLineParser parser( helpOpt, verboseOpt, quietOpt, inputOpt, outputOpt, outFormatOpt, inFormatOpt, solversOpt);

        if( ! parser.parse( argc, argv))
            return 1;

        if( helpOpt.isSet())
        {
            std::cout << helpText;

            return 0;
        }

        //
        // Set things up
        //
        Logger logger     = getLogger( verboseOpt, quietOpt);

        auto inputStream  = getInputStream( inputOpt, logger);
        auto reader       = getReader( inFormatOpt, *inputStream, logger);

        auto outputStream = getOutputStream( outputOpt, logger);
        auto writer       = getWriter( outFormatOpt, *outputStream, logger);

        auto solvers      = getSolvers( solversOpt, logger);

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

