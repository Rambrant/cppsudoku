//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <catch2/catch_test_macros.hpp>

#include "core/PluginRegistry.hpp"
#include "core/Logger.hpp"

using namespace com::rambrant::sudoku;

//
// Minimal plugin hierarchy — local to this translation unit only.
//
namespace
{
    struct ITestPlugin
    {
        explicit ITestPlugin( const Logger&) {}
        virtual ~ITestPlugin() = default;
    };

    //
    // Three concrete plugins declared in deliberately REVERSE alphabetical order
    // inside the TypeList.  The whole point of the first group of tests is to
    // prove that the registry re-orders them regardless.
    //
    struct ZebraPlugin final : ITestPlugin
    {
        static constexpr std::string_view pluginKey = "zebra";
        using ITestPlugin::ITestPlugin;
    };

    struct AlphaPlugin final : ITestPlugin
    {
        static constexpr std::string_view pluginKey = "alpha";
        using ITestPlugin::ITestPlugin;
    };

    struct MidPlugin final : ITestPlugin
    {
        static constexpr std::string_view pluginKey = "mid";
        using ITestPlugin::ITestPlugin;
    };

    //
    // A type that deliberately OMITS pluginKey — must not satisfy PluginType.
    //
    struct NotAPlugin final : ITestPlugin
    {
        using ITestPlugin::ITestPlugin;
    };

    //
    // TypeList in reverse-alpha order: zebra, alpha, mid.
    // keys() must still produce: alpha, mid, zebra.
    //
    using TestList = std::tuple<ZebraPlugin, AlphaPlugin, MidPlugin>;
}

//
// Concept
//

TEST_CASE( "PluginType: concept is satisfied by concrete plugins", "[unit][registry]")
{
    STATIC_REQUIRE(  PluginType<ZebraPlugin, ITestPlugin>);
    STATIC_REQUIRE(  PluginType<AlphaPlugin, ITestPlugin>);
    STATIC_REQUIRE(  PluginType<MidPlugin,   ITestPlugin>);
}

TEST_CASE( "PluginType: concept is not satisfied without pluginKey", "[unit][registry]")
{
    STATIC_REQUIRE_FALSE( PluginType<NotAPlugin,  ITestPlugin>);
}

TEST_CASE( "PluginType: concept is not satisfied by the abstract base itself", "[unit][registry]")
{
    STATIC_REQUIRE_FALSE( PluginType<ITestPlugin, ITestPlugin>);
}

//
// keys()
//
TEST_CASE( "PluginRegistry: keys() returns all registered entries", "[unit][registry]")
{
    const Logger                                 logger{ Logger::LogLevel::Quiet};
    const PluginRegistry<ITestPlugin, TestList>  registry;

    CHECK( registry.keys().size() == 3);
}

TEST_CASE( "PluginRegistry: keys() are sorted regardless of TypeList order", "[unit][registry]")
{
    //
    // TestList declares plugins as zebra, alpha, mid.
    // The sorted order must be alpha, mid, zebra.
    // This is the property that compile-time std::ranges::sort guarantees.
    //
    const Logger                                 logger{ Logger::LogLevel::Quiet};
    const PluginRegistry<ITestPlugin, TestList>  registry;

    REQUIRE( registry.keys() == std::vector<std::string>{ "alpha", "mid", "zebra"});
}

//
// create()
//

TEST_CASE( "PluginRegistry: create() returns a valid instance for every registered key", "[unit][registry]")
{
    const Logger                                logger{ Logger::LogLevel::Quiet};
    const PluginRegistry<ITestPlugin, TestList> registry;

    for( const auto& key : registry.keys())
    {
        CAPTURE( key);

        const auto result = registry.create( key, logger);

        REQUIRE( result.has_value());
        REQUIRE( result->get() != nullptr);
    }
}

TEST_CASE( "PluginRegistry: create() returns an error for an unknown key", "[unit][registry]")
{
    const Logger                                logger{ Logger::LogLevel::Quiet};
    const PluginRegistry<ITestPlugin, TestList> registry;

    const auto result = registry.create( "unknown", logger);

    REQUIRE_FALSE( result.has_value());
}

TEST_CASE( "PluginRegistry: create() error message contains the unknown key", "[unit][registry]")
{
    //
    // The error string must echo back the bad key so callers can produce
    // a useful diagnostic without inspecting the registry themselves.
    //
    const Logger                                logger{ Logger::LogLevel::Quiet};
    const PluginRegistry<ITestPlugin, TestList> registry;

    const auto result = registry.create( "doesnotexist", logger);

    REQUIRE_FALSE( result.has_value());
    CHECK( result.error().find( "doesnotexist") != std::string::npos);
}