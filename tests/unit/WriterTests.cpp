//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <LineWriter.hpp>
#include <Logger.hpp>
#include <sstream>
#include <SudokuTraits.hpp>
#include <catch2/catch_test_macros.hpp>

#include "BlockWriter.hpp"
#include "JsonWriter.hpp"
#include "PrettyWriter.hpp"

TEST_CASE( "Writers: LineWriter writes valid 9x9 boards", "[unit]")
{
    using namespace com::rambrant::sudoku;

    SudokuTraits::Board board{{
        std::array{5, 3, 0, 0, 7, 0, 0, 0, 0},
        std::array{6, 0, 0, 1, 9, 5, 0, 0, 0},
        std::array{0, 9, 8, 0, 0, 0, 0, 6, 0},
        std::array{8, 0, 0, 0, 6, 0, 0, 0, 3},
        std::array{4, 0, 0, 8, 0, 3, 0, 0, 1},
        std::array{7, 0, 0, 0, 2, 0, 0, 0, 6},
        std::array{0, 6, 0, 0, 0, 0, 2, 8, 0},
        std::array{0, 0, 0, 4, 1, 9, 0, 0, 5},
        std::array{0, 0, 0, 0, 8, 0, 0, 7, 9}
    }};

    SECTION( "LineWriter writes the board as a single line of space separated digits")
    {
        std::ostringstream stream;
        Logger             logger{};
        LineWriter         writer{ stream, logger};

        writer.write( board);

        REQUIRE( stream.str() == "5 3 0 0 7 0 0 0 0 6 0 0 1 9 5 0 0 0 0 9 8 0 0 0 0 6 0 8 0 0 0 6 0 0 0 3 4 0 0 8 0 3 0 0 1 7 0 0 0 2 0 0 0 6 0 6 0 0 0 0 2 8 0 0 0 0 4 1 9 0 0 5 0 0 0 0 8 0 0 7 9 ");
    }

    SECTION( "BlockWriter writes the board in 9 lines of 9 digits")
    {
        std::ostringstream stream;
        Logger             logger{};
        BlockWriter        writer{ stream, logger};

        writer.write( board);

        std::string expected =
            "5 3 0  0 7 0  0 0 0 \n"
            "6 0 0  1 9 5  0 0 0 \n"
            "0 9 8  0 0 0  0 6 0 \n"
            "\n"
            "8 0 0  0 6 0  0 0 3 \n"
            "4 0 0  8 0 3  0 0 1 \n"
            "7 0 0  0 2 0  0 0 6 \n"
            "\n"
            "0 6 0  0 0 0  2 8 0 \n"
            "0 0 0  4 1 9  0 0 5 \n"
            "0 0 0  0 8 0  0 7 9 \n";

        REQUIRE( stream.str() == expected);
    }

    SECTION( "PrettyWriter writes a decorated board with separators")
    {
        std::ostringstream stream;
        Logger             logger{};
        PrettyWriter       writer{ stream, logger};

        writer.write(board);

        std::string expected =
            "5 3 . | . 7 . | . . . \n"
            "6 . . | 1 9 5 | . . . \n"
            ". 9 8 | . . . | . 6 . \n"
            "------+-------+------\n"
            "8 . . | . 6 . | . . 3 \n"
            "4 . . | 8 . 3 | . . 1 \n"
            "7 . . | . 2 . | . . 6 \n"
            "------+-------+------\n"
            ". 6 . | . . . | 2 8 . \n"
            ". . . | 4 1 9 | . . 5 \n"
            ". . . | . 8 . | . 7 9 \n";

        REQUIRE(stream.str() == expected);
    }

    SECTION( "JsonWriter writes the board as a 2D JSON array")
    {
        std::ostringstream stream;
        Logger             logger{};
        JsonWriter         writer{ stream, logger};

        writer.write(board);

        std::string expected =
            "[\n"
            "  [ 5, 3, 0, 0, 7, 0, 0, 0, 0 ],\n"
            "  [ 6, 0, 0, 1, 9, 5, 0, 0, 0 ],\n"
            "  [ 0, 9, 8, 0, 0, 0, 0, 6, 0 ],\n"
            "  [ 8, 0, 0, 0, 6, 0, 0, 0, 3 ],\n"
            "  [ 4, 0, 0, 8, 0, 3, 0, 0, 1 ],\n"
            "  [ 7, 0, 0, 0, 2, 0, 0, 0, 6 ],\n"
            "  [ 0, 6, 0, 0, 0, 0, 2, 8, 0 ],\n"
            "  [ 0, 0, 0, 4, 1, 9, 0, 0, 5 ],\n"
            "  [ 0, 0, 0, 0, 8, 0, 0, 7, 9 ]\n"
            "]\n";

        REQUIRE( stream.str() == expected);
    }
}