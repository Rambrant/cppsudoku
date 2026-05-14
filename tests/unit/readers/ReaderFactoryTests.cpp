//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <sstream>
#include <catch2/catch_test_macros.hpp>

#include "readers/AsciiReader.hpp"
#include "readers/IReader.hpp"
#include "readers/JsonReader.hpp"
#include "core/Logger.hpp"
#include "readers/ReaderFactory.hpp"

using namespace com::rambrant::sudoku;

//
//  Concept
//
TEST_CASE( "ReaderPlugin: concept is satisfied by concrete readers", "[unit]")
{
    STATIC_REQUIRE(  ReaderPlugin<AsciiReader>);
    STATIC_REQUIRE(  ReaderPlugin<JsonReader>);
    STATIC_REQUIRE_FALSE( ReaderPlugin<IReader>);   // abstract base — no formatName
}

//
//  ReaderFactory
//
TEST_CASE( "ReaderFactory: formats() lists all registered reader keys", "[unit]")
{
    const auto formats = ReaderFactory::instance().formats();

    // std::flat_map sorts by key, so the order is deterministic
    REQUIRE( formats == std::vector<std::string>{ "json", "text"});
}

TEST_CASE( "ReaderFactory: create() returns a reader for every registered format", "[unit]")
{
    Logger logger{};

    for( const auto& format : ReaderFactory::instance().formats())
    {
        std::istringstream stream{};

        CAPTURE( format);
        const auto result = ReaderFactory::instance().create( format, stream, logger);

        REQUIRE( result.has_value());
    }
}

TEST_CASE( "ReaderFactory: create() returns an error for an unknown format", "[unit]")
{
    std::istringstream stream{};
    Logger             logger{};

    const auto result = ReaderFactory::instance().create( "csv", stream, logger);

    REQUIRE_FALSE( result.has_value());
    CHECK( result.error().find( "csv") != std::string::npos);
}