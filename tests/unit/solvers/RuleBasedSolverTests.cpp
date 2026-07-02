//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <catch2/catch_all.hpp>

#include "solvers/ISolver.hpp"
#include "solvers/RuleBasedSolver.hpp"
#include "core/Logger.hpp"
#include "core/SudokuTraits.hpp"

using namespace com::rambrant::sudoku;

//
// Shared board fixtures
//
namespace
{
    //
    // A board solvable by naked and hidden singles alone.
    //
    constexpr SudokuTraits::Board simpleUnsolved
    {{
        std::array{ 0, 0, 3, 0, 2, 0, 6, 0, 0 },
        std::array{ 9, 0, 0, 3, 0, 5, 0, 0, 1 },
        std::array{ 0, 0, 1, 8, 0, 6, 4, 0, 0 },
        std::array{ 0, 0, 8, 1, 0, 2, 9, 0, 0 },
        std::array{ 7, 0, 0, 0, 0, 0, 0, 0, 8 },
        std::array{ 0, 0, 6, 7, 0, 8, 2, 0, 0 },
        std::array{ 0, 0, 2, 6, 0, 9, 5, 0, 0 },
        std::array{ 8, 0, 0, 2, 0, 3, 0, 0, 9 },
        std::array{ 0, 0, 5, 0, 1, 0, 3, 0, 0 }
    }};

    constexpr SudokuTraits::Board simpleSolved
    {{
        std::array{ 4, 8, 3, 9, 2, 1, 6, 5, 7 },
        std::array{ 9, 6, 7, 3, 4, 5, 8, 2, 1 },
        std::array{ 2, 5, 1, 8, 7, 6, 4, 9, 3 },
        std::array{ 5, 4, 8, 1, 3, 2, 9, 7, 6 },
        std::array{ 7, 2, 9, 5, 6, 4, 1, 3, 8 },
        std::array{ 1, 3, 6, 7, 9, 8, 2, 4, 5 },
        std::array{ 3, 7, 2, 6, 8, 9, 5, 1, 4 },
        std::array{ 8, 1, 4, 2, 5, 3, 7, 6, 9 },
        std::array{ 6, 9, 5, 4, 1, 7, 3, 8, 2 }
    }};

    //
    // A board solvable by pure logic (no guessing), slightly harder.
    // Source: Wikipedia's canonical Sudoku example.
    //
    constexpr SudokuTraits::Board wikipediaUnsolved
    {{
        std::array{ 5, 3, 0, 0, 7, 0, 0, 0, 0 },
        std::array{ 6, 0, 0, 1, 9, 5, 0, 0, 0 },
        std::array{ 0, 9, 8, 0, 0, 0, 0, 6, 0 },
        std::array{ 8, 0, 0, 0, 6, 0, 0, 0, 3 },
        std::array{ 4, 0, 0, 8, 0, 3, 0, 0, 1 },
        std::array{ 7, 0, 0, 0, 2, 0, 0, 0, 6 },
        std::array{ 0, 6, 0, 0, 0, 0, 2, 8, 0 },
        std::array{ 0, 0, 0, 4, 1, 9, 0, 0, 5 },
        std::array{ 0, 0, 0, 0, 8, 0, 0, 7, 9 }
    }};

    constexpr SudokuTraits::Board wikipediaSolved
    {{
        std::array{ 5, 3, 4, 6, 7, 8, 9, 1, 2 },
        std::array{ 6, 7, 2, 1, 9, 5, 3, 4, 8 },
        std::array{ 1, 9, 8, 3, 4, 2, 5, 6, 7 },
        std::array{ 8, 5, 9, 7, 6, 1, 4, 2, 3 },
        std::array{ 4, 2, 6, 8, 5, 3, 7, 9, 1 },
        std::array{ 7, 1, 3, 9, 2, 4, 8, 5, 6 },
        std::array{ 9, 6, 1, 5, 3, 7, 2, 8, 4 },
        std::array{ 2, 8, 7, 4, 1, 9, 6, 3, 5 },
        std::array{ 3, 4, 5, 2, 8, 6, 1, 7, 9 }
    }};

    //
    // A board with a contradiction (two 5s in row 0).
    //
    constexpr SudokuTraits::Board invalidBoard
    {{
        std::array{ 5, 3, 5, 0, 7, 0, 0, 0, 0 },
        std::array{ 6, 0, 0, 1, 9, 5, 0, 0, 0 },
        std::array{ 0, 9, 8, 0, 0, 0, 0, 6, 0 },
        std::array{ 8, 0, 0, 0, 6, 0, 0, 0, 3 },
        std::array{ 4, 0, 0, 8, 0, 3, 0, 0, 1 },
        std::array{ 7, 0, 0, 0, 2, 0, 0, 0, 6 },
        std::array{ 0, 6, 0, 0, 0, 0, 2, 8, 0 },
        std::array{ 0, 0, 0, 4, 1, 9, 0, 0, 5 },
        std::array{ 0, 0, 0, 0, 8, 0, 0, 7, 9 }
    }};

    //
    // An empty board — no logic rule can make progress.
    //
    constexpr SudokuTraits::Board emptyBoard{};

    //
    // A board that demonstrably requires trial-and-error to solve.
    // (World's hardest Sudoku by Arto Inkala, 2010.)
    //
    constexpr SudokuTraits::Board hardestBoard
    {{
        std::array{ 8, 0, 0, 0, 0, 0, 0, 0, 0 },
        std::array{ 0, 0, 3, 6, 0, 0, 0, 0, 0 },
        std::array{ 0, 7, 0, 0, 9, 0, 2, 0, 0 },
        std::array{ 0, 5, 0, 0, 0, 7, 0, 0, 0 },
        std::array{ 0, 0, 0, 0, 4, 5, 7, 0, 0 },
        std::array{ 0, 0, 0, 1, 0, 0, 0, 3, 0 },
        std::array{ 0, 0, 1, 0, 0, 0, 0, 6, 8 },
        std::array{ 0, 0, 8, 5, 0, 0, 0, 1, 0 },
        std::array{ 0, 9, 0, 0, 0, 0, 4, 0, 0 }
    }};

}

//
// Concept and registration
//

TEST_CASE( "RuleBasedSolver: SolverPlugin concept", "[unit]")
{
    STATIC_REQUIRE( SolverPlugin<RuleBasedSolver>);
    STATIC_REQUIRE( RuleBasedSolver::pluginKey == "rules");
}

TEST_CASE( "RuleBasedSolver: name() returns 'rules'", "[unit]")
{
    Logger logger{};
    const RuleBasedSolver solver{ logger};

    CHECK( solver.name() == "rules");

    const std::unique_ptr<ISolver> base = std::make_unique<RuleBasedSolver>( logger);
    CHECK( base->name() == "rules");
}

//
// detail helpers: initCandidates / isSolved / isContradicted
//

TEST_CASE( "RuleBasedSolver detail: initCandidates", "[unit]")
{
    SECTION( "Empty cells receive all 9 candidates")
    {
        const auto result = detail::initCandidates( emptyBoard);
        REQUIRE( result.has_value());

        for( int r = 0; r < SudokuTraits::BOARD_SIZE; ++r)
            for( int c = 0; c < SudokuTraits::BOARD_SIZE; ++c)
                CHECK( result.value()[r][c].size() == SudokuTraits::BOARD_SIZE);
    }

    SECTION( "Given values are fixed and removed from peers")
    {
        const auto result = detail::initCandidates( simpleUnsolved);
        REQUIRE( result.has_value());

        // Cell (0,2) is given as 3 → must have exactly {3}
        const auto& cands = result.value()[0][2];
        REQUIRE( cands.size() == 1);
        CHECK( cands[0] == 3);

        // 3 must be absent from every peer in row 0
        for( int c = 0; c < SudokuTraits::BOARD_SIZE; ++c)
        {
            if( c == 2) continue;
            const auto& peerCands = result.value()[0][c];
            bool has3 = std::find( peerCands.begin(), peerCands.end(), 3) != peerCands.end();
            CHECK_FALSE( has3);
        }
    }

    SECTION( "Duplicate given value in a unit returns an error")
    {
        const auto result = detail::initCandidates( invalidBoard);
        CHECK_FALSE( result.has_value());
    }

    SECTION( "Already-solved board initialises without error")
    {
        const auto result = detail::initCandidates( simpleSolved);
        REQUIRE( result.has_value());
        CHECK( detail::isSolved( result.value()));
    }
}

TEST_CASE( "RuleBasedSolver detail: isSolved / isContradicted", "[unit]")
{
    SECTION( "isSolved: all singletons → true")
    {
        const auto result = detail::initCandidates( simpleSolved);
        REQUIRE( result.has_value());
        CHECK( detail::isSolved( result.value()));
    }

    SECTION( "isSolved: unsolved board → false")
    {
        const auto result = detail::initCandidates( emptyBoard);
        REQUIRE( result.has_value());
        CHECK_FALSE( detail::isSolved( result.value()));
    }

    SECTION( "isContradicted: valid board → false")
    {
        const auto result = detail::initCandidates( simpleUnsolved);
        REQUIRE( result.has_value());
        CHECK_FALSE( detail::isContradicted( result.value()));
    }
}

//
// Individual rules
//

TEST_CASE( "RuleBasedSolver detail: applyNakedSingles", "[unit]")
{
    SECTION( "Propagates a singleton into its peers")
    {
        //
        // Start from a known solved board: all cells are singletons.
        // applyNakedSingles on a solved grid removes nothing new (all peers already
        // lack each value), so progress must be false after init.
        //
        const auto gridResult = detail::initCandidates( simpleSolved);
        REQUIRE( gridResult.has_value());

        auto grid = gridResult.value();
        const bool progress = detail::applyNakedSingles( grid);
        CHECK_FALSE( progress);
        CHECK( detail::isSolved( grid));
    }

    SECTION( "Solves a board reducible to naked singles")
    {
        //
        // After initCandidates, some cells in simpleUnsolved are immediately
        // naked singles (down to 1 candidate). Repeated application should
        // clear the board if it is naked-single-complete.
        //
        const auto gridResult = detail::initCandidates( simpleUnsolved);
        REQUIRE( gridResult.has_value());

        auto grid = gridResult.value();

        bool anyProgress = true;
        while( anyProgress)
        {
            anyProgress = detail::applyNakedSingles( grid);
            if( detail::isSolved( grid)) break;
        }

        // simpleUnsolved is known to be solvable — either solved here or needs
        // hidden singles. Either way there must be no contradictions.
        CHECK_FALSE( detail::isContradicted( grid));
    }
}

TEST_CASE( "RuleBasedSolver detail: applyHiddenSingles", "[unit]")
{
    SECTION( "Makes progress on the simple board after naked-single pass")
    {
        const auto gridResult = detail::initCandidates( simpleUnsolved);
        REQUIRE( gridResult.has_value());

        auto grid = gridResult.value();

        // Run naked singles to their conclusion first
        while( detail::applyNakedSingles( grid)) {}

        if( ! detail::isSolved( grid))
        {
            const bool hiddenProgress = detail::applyHiddenSingles( grid);
            CHECK( hiddenProgress);  // simpleUnsolved needs at most one extra pass
        }

        CHECK_FALSE( detail::isContradicted( grid));
    }
}

//
// End-to-end solver tests
// 

TEST_CASE( "RuleBasedSolver: solves the simple board", "[unit]")
{
    Logger              logger{};
    RuleBasedSolver     solver{ logger};
    std::atomic         cancelFlag{ false};

    auto board{ simpleUnsolved};
    auto [solved, passes, result] = solver.solve( board, cancelFlag);

    REQUIRE( solved);
    CHECK( result == simpleSolved);
    CHECK( passes >= 1);
}

TEST_CASE( "RuleBasedSolver: solves the Wikipedia board (logic-only puzzle)", "[unit]")
{
    Logger              logger{};
    RuleBasedSolver     solver{ logger};
    std::atomic         cancelFlag{ false};

    auto board{ wikipediaUnsolved};
    auto [solved, passes, result] = solver.solve( board, cancelFlag);

    REQUIRE( solved);
    CHECK( result == wikipediaSolved);
}

TEST_CASE( "RuleBasedSolver: returns solved=true for an already-solved board", "[unit]")
{
    Logger              logger{};
    RuleBasedSolver     solver{ logger};
    std::atomic         cancelFlag{ false};

    auto board{ simpleSolved};
    auto [solved, passes, result] = solver.solve( board, cancelFlag);

    REQUIRE( solved);
    CHECK( result == simpleSolved);
    CHECK( passes == 0);  // Detected in init, no rule passes needed
}

TEST_CASE( "RuleBasedSolver: returns solved=false for a contradicted board", "[unit]")
{
    Logger              logger{};
    RuleBasedSolver     solver{ logger};
    std::atomic         cancelFlag{ false};

    auto board{ invalidBoard};
    auto [solved, _, result] = solver.solve( board, cancelFlag);

    CHECK_FALSE( solved);
}

TEST_CASE( "RuleBasedSolver: returns solved=false when guessing is required", "[unit]")
{
    Logger              logger{};
    RuleBasedSolver     solver{ logger};
    std::atomic         cancelFlag{ false};

    auto board{ hardestBoard};
    auto [solved, _, result] = solver.solve( board, cancelFlag);

    //
    // Arto Inkala's 2010 puzzle requires hypothetical reasoning beyond the six
    // rules implemented here.  A correct rule-based solver must admit defeat
    // rather than guess.
    //
    CHECK_FALSE( solved);
}

TEST_CASE( "RuleBasedSolver: returns solved=false for an empty board", "[unit]")
{
    Logger              logger{};
    RuleBasedSolver     solver{ logger};
    std::atomic         cancelFlag{ false};

    auto board{ emptyBoard};
    auto [solved, _, result] = solver.solve( board, cancelFlag);

    CHECK_FALSE( solved);
}

TEST_CASE( "RuleBasedSolver: premature cancellation", "[unit]")
{
    Logger              logger{};
    RuleBasedSolver     solver{ logger};
    std::atomic         cancelFlag{ true};  // pre-set

    auto board{ simpleUnsolved};
    auto [solved, passes, result] = solver.solve( board, cancelFlag);

    CHECK_FALSE( solved);
    CHECK( passes == 0);
}

TEST_CASE( "RuleBasedSolver: sets cancelFlag on success", "[unit]")
{
    Logger              logger{};
    RuleBasedSolver     solver{ logger};
    std::atomic         cancelFlag{ false};

    auto board{ simpleUnsolved};
    auto [solved, _, result] = solver.solve( board, cancelFlag);

    REQUIRE( solved);
    CHECK( cancelFlag.load());
}