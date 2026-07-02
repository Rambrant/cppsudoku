//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <sstream>
#include <catch2/catch_test_macros.hpp>

#include "core/Logger.hpp"
#include "solvers/ISolver.hpp"
#include "solvers/BackTrackingSolver.hpp"
#include "solvers/ConstraintPropagationSolver.hpp"
#include "solvers/SolverFactory.hpp"

using namespace com::rambrant::sudoku;

//
//  Concept
//
TEST_CASE( "SolverPlugin: concept is satisfied by concrete solvers", "[unit]")
{
    STATIC_REQUIRE(  SolverPlugin<BackTrackingSolver>);
    STATIC_REQUIRE(  SolverPlugin<ConstraintPropagationSolver>);
    STATIC_REQUIRE_FALSE( SolverPlugin<ISolver>);   // abstract base — no pluginKey
}

//
//  SolverFactory
//
TEST_CASE( "SolverFactory: pluginKeys() lists all registered solver keys", "[unit]")
{
    const auto names = SolverFactory::instance().pluginKeys();

    REQUIRE( names == std::vector<std::string>{ "backtracking", "constraint", "rules"});
}

TEST_CASE( "SolverFactory: create() returns a solver for every registered name", "[unit]")
{
    Logger logger{};

    for( const auto& name : SolverFactory::instance().pluginKeys())
    {
        CAPTURE( name);
        auto result = SolverFactory::instance().create( name, logger);

        REQUIRE( result.has_value());

        // The created solver must report the same name it was looked up by
        CHECK( (*result)->name() == name);
    }
}

TEST_CASE( "SolverFactory: create() returns an error for an unknown name", "[unit]")
{
    Logger logger{};

    auto result = SolverFactory::instance().create( "bruteforce", logger);

    REQUIRE_FALSE( result.has_value());
    CHECK( result.error().find( "bruteforce") != std::string::npos);
}

TEST_CASE( "SolverFactory: pluginKeys() are in ascending order", "[unit]")
{
    const auto formats = SolverFactory::instance().pluginKeys();

    REQUIRE( std::ranges::is_sorted( formats));
}
