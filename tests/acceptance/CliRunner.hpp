//
// Created by Thomas Rambrant on 2025-06-04.
//
#pragma once

#include <array>
#include <filesystem>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <sys/wait.h>

class CliRunner
{
    public:
        explicit CliRunner( std::string executablePath, std::string workingDir = {});

        auto run(
            const std::vector<std::string>& args       = {},
            const std::string&              stdinInput = "") -> int;

        auto output() const -> const std::string&;
        auto exitCode() const -> int;
        auto outputContains( const std::string & str) const -> bool;
        auto countOutputMatches( const std::regex & pattern ) -> int;

    private:

        std::string mWorkingDir;
        std::string mExecutable;
        std::string mOutput;
        int         mExitCode;
};

namespace details
{
    inline auto escapeShellArg( const std::string & arg ) -> std::string
    {
        std::ostringstream oss;
        oss << "'";
        for (char c : arg) {
            if (c == '\'') oss << "'\\''";
            else oss << c;
        }
        oss << "'";
        return oss.str();
    }

    inline auto escapeQuotes( const std::string & input ) -> std::string
    {
        std::string result;
        for (char c : input) {
            if (c == '"') result += "\\\"";
            else result += c;
        }
        return result;
    }
}

inline CliRunner::CliRunner( std::string executablePath, std::string workingDir) :
    mWorkingDir( std::move(workingDir)),
    mExecutable( std::move( executablePath)),
    mExitCode( -1)
{}

inline auto CliRunner::run(
    const std::vector<std::string>& args,
    const std::string&              stdinInput) -> int
{
    std::ostringstream cmd;

    if( ! mWorkingDir.empty())
    {
        cmd << "cd " << mWorkingDir << " && ";
    }

    cmd << mExecutable;

    for( const auto& arg : args)
    {
        cmd << " " << details::escapeShellArg( arg);
    }

    if( ! stdinInput.empty())
    {
        cmd << " <<< \"" << details::escapeQuotes( stdinInput) << "\"";
    }

    cmd << " 2>&1"; // Redirect stderr to stdout

    std::string commandStr = cmd.str();
    mOutput.clear();

    std::array<char, 256> buffer{};
    std::unique_ptr<FILE, decltype(&pclose)> pipe( popen( commandStr.c_str(), "r"), pclose);

    if( ! pipe)
        throw std::runtime_error( "Failed to execute: " + commandStr);

    while( fgets( buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        mOutput += buffer.data();
    }

    auto rawExitCode = pclose( pipe.get());

    mExitCode = WIFEXITED( rawExitCode) ? WEXITSTATUS( rawExitCode): -1; // Extract the real exit code if abnormal exit

    return mExitCode;
}

inline auto CliRunner::output() const -> const std::string&
{
    return mOutput;
}

inline auto CliRunner::exitCode() const -> int
{
    return mExitCode;
}

inline auto CliRunner::outputContains( const std::string & str ) const -> bool
{
    return mOutput.find( str) != std::string::npos;
}

inline auto CliRunner::countOutputMatches( const std::regex & pattern ) -> int
{
    return static_cast<int>( std::distance(
        std::sregex_iterator(mOutput.begin(), mOutput.end(), pattern),
        std::sregex_iterator()
    ));
}
