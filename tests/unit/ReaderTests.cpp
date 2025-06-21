//
// Created by Thomas Rambrant on 2025-06-18.
//
#include <regex>
#include <sstream>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_contains.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "AsciiReader.hpp"
#include "JsonReader.hpp"
#include "Logger.hpp"

using namespace com::rambrant::sudoku;

TEST_CASE( "Readers: AsciiReader reads valid 9x9 boards", "unit")
{
    SECTION( "AsciiReader accepts clean 81-digit input")
    {
        const std::string input =
            "123456789"
            "456789123"
            "789123456"
            "234567891"
            "567891234"
            "891234567"
            "345678912"
            "678912345"
            "912345678";

        std::istringstream stream( input);
        Logger             logger{};
        AsciiReader        reader( stream, logger);

        auto board = reader.read();

        REQUIRE(board.size() == 9);

        for( const auto& row : board)
        {
            CHECK( row.size() == 9);
        }

        CHECK( board[0][0] == 1);
        CHECK( board[8][8] == 8);
    }

    SECTION( "AsciiReader handles input with whitespace and newlines")
    {
        std::string input =
            "1 2 3 4 5 6 7 8 9\n"
            "4 5 6 7 8 9 1 2 3\n"
            "7 8 9 1 2 3 4 5 6\n"
            "2 3 4 5 6 7 8 9 1\n"
            "5 6 7 8 9 1 2 3 4\n"
            "8 9 1 2 3 4 5 6 7\n"
            "3 4 5 6 7 8 9 1 2\n"
            "6 7 8 9 1 2 3 4 5\n"
            "9 1 2 3 4 5 6 7 8";

        std::istringstream stream( input);
        Logger       logger{};
        AsciiReader  reader( stream, logger);

        auto board = reader.read();

        CHECK( board[0][0] == 1);
        CHECK( board[8][8] == 8);
    }

    SECTION( "AsciiReader reads input up to 81 digits")
    {
        std::string input = std::string( 81, '1') + "Extra";

        std::istringstream stream( input);
        Logger             logger{};
        AsciiReader reader( stream, logger);

        [[maybe_unused]] auto board = reader.read();

        char leftover;
        stream >> leftover;

        CHECK_FALSE( stream.eof()); // We expect more characters after read
        CHECK( leftover == 'E');
    }

    SECTION( "AsciiReader ignores non-digit characters")
    {
        std::string input = R"(This is a comment!
        1 2 3 | 4 5 6 | 7 8 9
        4 5 6 | 7 8 9 | 1 2 3
        7 8 9 | 1 2 3 | 4 5 6
        ------+------+------
        2 3 4 | 5 6 7 | 8 9 1
        5 6 7 | 8 9 1 | 2 3 4
        8 9 1 | 2 3 4 | 5 6 7
        ------+------+------
        3 4 5 | 6 7 8 | 9 1 2
        6 7 8 | 9 1 2 | 3 4 5
        9 1 2 | 3 4 5 | 6 7 8)";

        std::istringstream stream( input);
        Logger             logger{};
        AsciiReader reader( stream, logger);

        auto board = reader.read();

        CHECK( board[0][0] == 1);
        CHECK( board[8][8] == 8);
    }
}

TEST_CASE( "Readers: AsciiReader rejects input with fewer than 81 digits")
{
    std::string input = "123456789"; // too short

    std::istringstream stream( input);
    Logger             logger{};
    AsciiReader reader(stream, logger);

    REQUIRE_THROWS_AS( reader.read(), std::runtime_error);
}

TEST_CASE( "Readers: JsonReader reads valid 9x9 boards", "unit")
{
    std::string json = R"(
    [
        [ 4, 0, 0, 0, 0, 0, 8, 0, 5 ],
        [ 0, 3, 0, 0, 0, 0, 0, 0, 0 ],
        [ 0, 0, 0, 7, 0, 0, 0, 0, 0 ],
        [ 0, 2, 0, 0, 0, 0, 0, 6, 0 ],
        [ 0, 0, 0, 0, 8, 0, 4, 0, 0 ],
        [ 0, 0, 0, 0, 1, 0, 0, 0, 0 ],
        [ 0, 0, 0, 6, 0, 3, 0, 7, 0 ],
        [ 5, 0, 0, 2, 0, 0, 0, 0, 0 ],
        [ 1, 0, 4, 0, 0, 0, 0, 0, 0 ]
    ]
    )";

    SECTION( "JsonReader reads a grid of integers")
    {
        std::istringstream input( json);
        Logger             logger{};
        JsonReader         reader( input, logger);

        REQUIRE_NOTHROW( reader.read());
    }

    SECTION( "JsonReader reads a grid of string literals")
    {
        std::regex digitPattern(R"(([0-9]))");
        json = std::regex_replace( json, digitPattern, R"("$1")");

        std::istringstream input( json);
        Logger             logger{};
        JsonReader         reader( input, logger);

        REQUIRE_NOTHROW( reader.read());
    }

    SECTION( "JsonReader reads a grid of string literals with zero replaced by dot")
    {
        std::regex digitPattern(R"(([0-9]))");
        std::regex zeroPattern(R"(0)");

        json = std::regex_replace( json, digitPattern, R"("$1")");
        json = std::regex_replace( json, zeroPattern, R"(.)");

        std::istringstream input( json);
        Logger             logger{};
        JsonReader         reader( input, logger);

        REQUIRE_NOTHROW( reader.read());
    }
}

TEST_CASE( "Readers: JsonReader throws on illegal input")
{
    SECTION( "JsonReader throws on too few rows")
    {
        std::string json = R"([[1,2,3,4,5,6,7,8,9]])";

        std::istringstream input( json);
        Logger             logger{};
        JsonReader         reader( input, logger);

        REQUIRE_THROWS_WITH( reader.read(), Catch::Matchers::ContainsSubstring("Wrong number of rows"));
    }

    SECTION( "JsonReader throws on a row with too few columns")
    {
        std::string json = R"(
        [
            [1,2,3,4,5,6,7,8],
            [1,2,3,4,5,6,7,8,9],
            [1,2,3,4,5,6,7,8,9],
            [1,2,3,4,5,6,7,8,9],
            [1,2,3,4,5,6,7,8,9],
            [1,2,3,4,5,6,7,8,9],
            [1,2,3,4,5,6,7,8,9],
            [1,2,3,4,5,6,7,8,9],
            [1,2,3,4,5,6,7,8,9]
        ]
        )";

        std::istringstream input( json);
        Logger             logger{};
        JsonReader         reader( input, logger);

        REQUIRE_THROWS_WITH( reader.read(), Catch::Matchers::ContainsSubstring( "Wrong number of columns"));
    }

    SECTION( "JsonReader throws on invalid cell value")
    {
        std::string json = R"(
        [
            [1,2,3,4,5,6,7,8,9],
            [1,2,3,4,5,6,7,8,9],
            [1,2,3,4,5,6,7,8,9],
            [1,2,3,4,5,6,7,8,9],
            [1,2,3,4,5,6,7,8,9],
            [1,2,3,4,5,6,7,8,9],
            [1,2,3,4,5,6,7,8,9],
            [1,2,3,4,5,6,7,8,9],
            [1,2,3,4,5,6,7,8,10]
        ]
        )";

        std::istringstream input( json);
        Logger             logger{};
        JsonReader         reader( input, logger);

        REQUIRE_THROWS_WITH( reader.read(), Catch::Matchers::ContainsSubstring( "Invalid value"));
    }

    SECTION(" JsonReader throws on invalid JSON format")
    {
        std::string json = R"(not even json)";

        std::istringstream input( json);
        Logger             logger{};
        JsonReader         reader( input, logger);

        REQUIRE_THROWS( reader.read());
    }
}
