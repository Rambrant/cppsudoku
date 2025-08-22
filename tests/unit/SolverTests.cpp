//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <catch2/catch_all.hpp>

#include "BackTrackingSolver.hpp"
#include "ConstraintPropagationSolver.hpp"
#include "Logger.hpp"
#include "SudokuTraits.hpp"

using namespace com::rambrant::sudoku;

namespace
{
    //
    // An unresolved board to solve
    //
    constexpr SudokuTraits::Board unsolvedBoard
    {{
        std::array{ 0, 0, 3, 0, 2, 0, 6, 0, 0},
        std::array{ 9, 0, 0, 3, 0, 5, 0, 0, 1},
        std::array{ 0, 0, 1, 8, 0, 6, 4, 0, 0},
        std::array{ 0, 0, 8, 1, 0, 2, 9, 0, 0},
        std::array{ 7, 0, 0, 0, 0, 0, 0, 0, 8},
        std::array{ 0, 0, 6, 7, 0, 8, 2, 0, 0},
        std::array{ 0, 0, 2, 6, 0, 9, 5, 0, 0},
        std::array{ 8, 0, 0, 2, 0, 3, 0, 0, 9},
        std::array{ 0, 0, 5, 0, 1, 0, 3, 0, 0}
    }};

    //
    // The solved board
    //
    constexpr SudokuTraits::Board solvedBoard
    {{
        std::array{ 4, 8, 3, 9, 2, 1, 6, 5, 7},
        std::array{ 9, 6, 7, 3, 4, 5, 8, 2, 1},
        std::array{ 2, 5, 1, 8, 7, 6, 4, 9, 3 },
        std::array{ 5, 4, 8, 1, 3, 2, 9, 7, 6 },
        std::array{ 7, 2, 9, 5, 6, 4, 1, 3, 8 },
        std::array{ 1, 3, 6, 7, 9, 8, 2, 4, 5 },
        std::array{ 3, 7, 2, 6, 8, 9, 5, 1, 4 },
        std::array{ 8, 1, 4, 2, 5, 3, 7, 6, 9 },
        std::array{ 6, 9, 5, 4, 1, 7, 3, 8, 2 }
    }};

    //
    // A hard board to solve (contains guesses)
    //
    const SudokuTraits::Board hardBoard
    {{
        std::array{ 5, 3, 0, 0, 7, 0, 0, 0, 0},
        std::array{ 6, 0, 0, 1, 9, 5, 0, 0, 0},
        std::array{ 0, 9, 8, 0, 0, 0, 0, 6, 0},
        std::array{ 8, 0, 0, 0, 6, 0, 0, 0, 3},
        std::array{ 4, 0, 0, 8, 0, 3, 0, 0, 1},
        std::array{ 7, 0, 0, 0, 2, 0, 0, 0, 6},
        std::array{ 0, 6, 0, 0, 0, 0, 2, 8, 0},
        std::array{ 0, 0, 0, 4, 1, 9, 0, 0, 5},
        std::array{ 0, 0, 0, 0, 8, 0, 0, 7, 9}
    }};

    //
    // The solved hard board
    //
    constexpr SudokuTraits::Board solvedHardBoard
    {{
        std::array{ 5, 3, 4, 6, 7, 8, 9, 1, 2},
        std::array{ 6, 7, 2, 1, 9, 5, 3, 4, 8},
        std::array{ 1, 9, 8, 3, 4, 2, 5, 6, 7},
        std::array{ 8, 5, 9, 7, 6, 1, 4, 2, 3},
        std::array{ 4, 2, 6, 8, 5, 3, 7, 9, 1},
        std::array{ 7, 1, 3, 9, 2, 4, 8, 5, 6},
        std::array{ 9, 6, 1, 5, 3, 7, 2, 8, 4},
        std::array{ 2, 8, 7, 4, 1, 9, 6, 3, 5},
        std::array{ 3, 4, 5, 2, 8, 6, 1, 7, 9}
    }};

    //
    // A board with a contradiction (duplicate 5's in top row)
    //
    constexpr SudokuTraits::Board invalidBoard
    {{
        std::array{ 5, 3, 5, 0, 7, 0, 0, 0, 0},
        std::array{ 6, 0, 0, 1, 9, 5, 0, 0, 0},
        std::array{ 0, 9, 8, 0, 0, 0, 0, 6, 0},
        std::array{ 8, 0, 0, 0, 6, 0, 0, 0, 3},
        std::array{ 4, 0, 0, 8, 0, 3, 0, 0, 1},
        std::array{ 7, 0, 0, 0, 2, 0, 0, 0, 6},
        std::array{ 0, 6, 0, 0, 0, 0, 2, 8, 0},
        std::array{ 0, 0, 0, 4, 1, 9, 0, 0, 5},
        std::array{ 0, 0, 0, 0, 8, 0, 0, 7, 9}
    }};

    //
    // An empty board (all zeroes)
    //
    constexpr SudokuTraits::Board emptyBoard
    {{
        std::array{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array{ 0, 0, 0, 0, 0, 0, 0, 0, 0},
        std::array{ 0, 0, 0, 0, 0, 0, 0, 0, 0}
    }};

    //
    // Template helper for testing both solvers
    //
    template< typename SolverType>
    void testSolverImpl( const std::map<std::string, bool>& args)
    {
        Logger     logger{};
        SolverType solver{ logger};

        SECTION( "Solves a board", "[unit]")
        {
            std::atomic cancelFlag( false);
            auto orgBoard{ unsolvedBoard};
            auto [result, _, board] = solver.solve( orgBoard, cancelFlag);

            REQUIRE( result == args.at( "solvesUnsolved"));

            if( result) REQUIRE( board == solvedBoard);
        }

        SECTION( "Solves a hard board with guesses", "[unit]")
        {
            std::atomic cancelFlag( false);
            auto orgBoard{ hardBoard};
            auto [result, _, board] = solver.solve( orgBoard, cancelFlag);

            REQUIRE( result == args.at( "solvesHardBoard"));
            if( result) REQUIRE( board == solvedHardBoard);
        }

        SECTION( "Solves an already solved board without modifying it")
        {
            std::atomic cancelFlag( false);
            auto orgBoard{ solvedBoard};
            auto [result, _, board] = solver.solve( orgBoard, cancelFlag);

            REQUIRE( result == args.at( "solvesSolved"));
            if( result) REQUIRE( board == solvedBoard);
        }

        SECTION( "Gracefully fail to solve a board with contradiction")
        {
            std::atomic cancelFlag( false);
            auto orgBoard{ invalidBoard};
            auto [result, _, board] = solver.solve( orgBoard, cancelFlag);

            REQUIRE( result == args.at( "solverContradicted"));
        }

        SECTION( "Solves an empty board (brute force or by deduction)")
        {
            std::atomic cancelFlag( false);
            auto orgBoard{ emptyBoard};
            auto [result, _, board] = solver.solve( orgBoard, cancelFlag);

            REQUIRE( result == args.at( "solvesEmpty"));

            // Optionally check a few cells are non-zero
            size_t filled = 0;
            for( const auto& row : board)
            {
                for( int val : row)
                {
                    if( val != 0)
                    {
                        filled++;
                    }
                }
            }

            REQUIRE( filled > 0);
        }
    }
}

TEST_CASE( "Solvers: BacktrackingSolver", "[unit]")
{
    testSolverImpl< BackTrackingSolver>( {
        { "solvesUnsolved",     true },
        { "solvesHardBoard",    false},
        { "solvesSolved",       true},
        { "solverContradicted", false},
        { "solvesEmpty",        true}}
    );
}

TEST_CASE( "Solvers: ConstraintPropagationSolver", "[unit]")
{
    testSolverImpl< ConstraintPropagationSolver>({
        { "solvesUnsolved",     true },
        { "solvesHardBoard",    true},
        { "solvesSolved",       true},
        { "solverContradicted", false},
        { "solvesEmpty",        true}}
    );
}

TEST_CASE( "Solvers: Premature Exit", "[unit]")
{
    SECTION( "Exit BackTracking Solver", "[unit]")
    {
        std::atomic        cancelFlag( true);
        Logger             logger{};
        BackTrackingSolver solver = BackTrackingSolver( logger);

        auto orgBoard{ unsolvedBoard};
        auto [result, recursions, board] = solver.solve( orgBoard, cancelFlag);

        REQUIRE( result == false);
        REQUIRE( recursions == 0);
    }

    SECTION( "Exit ConstraintPropagation Solver", "[unit]")
    {
        std::atomic                 cancelFlag( true);
        Logger                      logger{};
        ConstraintPropagationSolver solver = ConstraintPropagationSolver( logger);

        auto orgBoard{ unsolvedBoard};
        auto [result, recursions, board] = solver.solve( orgBoard, cancelFlag);

        REQUIRE( result == false);
        REQUIRE( recursions == 0);
    }
}