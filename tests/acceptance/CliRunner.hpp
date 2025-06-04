//
// Created by Thomas Rambrant on 2025-06-04.
//
#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <array>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <filesystem>

class CliRunner
{
    public:
        explicit CliRunner( std::string executablePath);

        auto run(
            const std::vector<std::string>& args       = {},
            const std::string&              stdinInput = "",
            const std::filesystem::path&    workingDir = {}) -> int;

        auto output() const -> const std::string&;
        auto exitCode() const -> int;
        auto outputContains( const std::string & str) const -> bool;

    private:

        std::string mExecutable;
        std::string mOutput;
        int         mExitCode;
};

namespace
{
    auto escapeShellArg( const std::string & arg ) -> std::string
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

    auto escapeQuotes( const std::string & input ) -> std::string
    {
        std::string result;
        for (char c : input) {
            if (c == '"') result += "\\\"";
            else result += c;
        }
        return result;
    }
}

inline CliRunner::CliRunner(std::string executablePath) :
    mExecutable( std::move( executablePath)),
    mExitCode( -1)
{}

inline auto CliRunner::run(
    const std::vector<std::string>& args,
    const std::string&              stdinInput,
    const std::filesystem::path&    workingDir) -> int
{
    std::ostringstream cmd;
    if (!workingDir.empty())
    {
        cmd << "cd " << workingDir << " && ";
    }

    cmd << mExecutable;
    for (const auto& arg : args)
    {
        cmd << " " << escapeShellArg(arg);
    }

    if (!stdinInput.empty())
    {
        cmd << " <<< \"" << escapeQuotes(stdinInput) << "\"";
    }

    cmd << " 2>&1"; // Redirect stderr to stdout

    std::string commandStr = cmd.str();
    mOutput.clear();

    std::array<char, 256> buffer{};
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(commandStr.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("Failed to execute: " + commandStr);

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        mOutput += buffer.data();
    }

    mExitCode = pclose(pipe.get());
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
