//
// Created by Thomas Rambrant on 2025-06-17.
//
#include <catch2/catch_test_macros.hpp>
#include "FileStream.hpp"
#include <fstream>
#include <string>
#include <filesystem>

using com::rambrant::sudoku::FileStream;

using     Mode = FileStream::Mode;
namespace fs   = std::filesystem;

TEST_CASE( "FileStream: opens, reads and writes to a file", "[unit]")
{
    const auto tmpDir   = fs::temp_directory_path();
    const auto filePath = tmpDir / "sudoku_test_output.txt";

    SECTION( "Opens a file in Write mode and writes data")
    {
        {
            FileStream file( filePath.string(), Mode::Write);
            REQUIRE( file.is_open());

            file << "Logging test line" << std::endl;
        }

        std::ifstream check( filePath);
        std::string line;
        std::getline( check, line);

        REQUIRE( line == "Logging test line");
    }

    SECTION( "Opens an existing file in Read mode and reads data")
    {
        {
            std::ofstream setup( filePath);
            setup << "Read mode line" << std::endl;
        }

        FileStream file( filePath.string(), Mode::Read);
        REQUIRE( file.is_open());

        std::string line;
        std::getline( file, line);

        REQUIRE( line == "Read mode line");
    }

    SECTION( "Opens file in ReadWrite mode and does both operations")
    {
        {
            FileStream file( filePath.string(), Mode::ReadWrite);
            REQUIRE( file.is_open());

            file << "Combined read/write" << std::endl;

            file.seekg( 0);

            std::string line;
            std::getline( file, line);

            REQUIRE( line == "Combined read/write");
        }

        FileStream file( filePath.string(), Mode::ReadWrite);
        REQUIRE( file.is_open());

        std::string line;
        std::getline( file, line);

        REQUIRE( line == "Combined read/write");
    }

    fs::remove( filePath);  // Clean up
}

TEST_CASE( "FileStream: fail gracefully when file doesn't exist in Read mode", "[FileStream]")
{
    const auto badPath = fs::temp_directory_path() / "nonexistent_input.txt";

    REQUIRE_THROWS_AS( FileStream( badPath.string(), Mode::Read), std::runtime_error);
}