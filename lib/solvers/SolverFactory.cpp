//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "SolverFactory.hpp"

#include <algorithm>

#include "core/Logger.hpp"
#include "SolverList.hpp"   // ← delete this line when migrating to P2996

namespace com::rambrant::sudoku
{
    namespace
    {
        using CreatorFn = std::unique_ptr<ISolver>(*)( const Logger&);

        struct RegistryEntry
        {
            std::string_view name;
            CreatorFn        creator;
        };

        //
        // Builds a sorted constexpr array of RegistryEntry from a parameter pack
        // of SolverPlugin types.
        //
        // Each non-capturing lambda is implicitly converted to a function pointer
        // at compile time. std::sort is constexpr since C++20. The result lives
        // in .rodata — no heap allocation, zero runtime initialization cost.
        //
        // P2996 MIGRATION
        //
        // When Clang supports C++26 static reflection (P2996 + P1306):
        //
        // Step 1 — delete the #include "SolverList.hpp" at the top of this file.
        //
        // Step 2 — replace buildRegistry, buildRegistryFromTuple and gSolverRegistry with:
        //
        //   consteval auto buildRegistry()
        //   {
        //       constexpr std::size_t count = []
        //       {
        //           std::size_t n = 0;
        //           for ( auto r : std::meta::members_of( ^com::rambrant::sudoku))
        //               if ( std::meta::is_type( r) && SolverPlugin<[:r:]>) ++n;
        //           return n;
        //       }();
        //
        //       std::array<RegistryEntry, count> entries{};
        //       std::size_t i = 0;
        //       template for ( constexpr auto r : std::meta::members_of( ^com::rambrant::sudoku))
        //       {
        //           if constexpr ( SolverPlugin<[:r:]>)
        //               entries[i++] = { [:r:]::solverName,
        //                                []( const Logger& l) -> std::unique_ptr<ISolver>
        //                                { return std::make_unique<[:r:]>( l); } };
        //       }
        //       std::sort( entries.begin(), entries.end(),
        //           []( const auto& a, const auto& b) { return a.name < b.name; });
        //       return entries;
        //   }
        //
        //   constexpr auto gSolverRegistry = buildRegistry();
        //
        template<SolverPlugin... Ts>
        constexpr auto buildRegistry() -> std::array<RegistryEntry, sizeof...(Ts)>
        {
            std::array<RegistryEntry, sizeof...(Ts)> entries{{
                RegistryEntry{
                    Ts::solverName,
                    []( const Logger& logger) -> std::unique_ptr<ISolver>
                    {
                        return std::make_unique<Ts>( logger);
                    }
                }...
            }};

            std::sort( entries.begin(), entries.end(),
                []( const RegistryEntry& a, const RegistryEntry& b)
                {
                    return a.name < b.name;
                });

            return entries;
        }

        template<typename... Ts>
        constexpr auto buildRegistryFromTuple( std::type_identity<std::tuple<Ts...>>)
        {
            return buildRegistry<Ts...>();
        }

        //
        // The registry — a constexpr array embedded in .rodata.
        // Built and sorted at compile time from SolverList.
        //
        constexpr auto gSolverRegistry = buildRegistryFromTuple(
            std::type_identity<SolverList>{});

    } // anonymous namespace

    //
    // Singleton
    //
    // The factory carries no mutable state — the constructor is trivial.
    // The singleton exists solely to preserve the existing call-site interface.
    //
    auto SolverFactory::instance() -> const SolverFactory&
    {
        static const SolverFactory instance;
        return instance;
    }

    auto SolverFactory::create( std::string_view name, const Logger& logger) const
        -> std::expected<std::unique_ptr<ISolver>, std::string>
    {
        const auto it = std::lower_bound(
            gSolverRegistry.begin(),
            gSolverRegistry.end(),
            name,
            []( const RegistryEntry& entry, std::string_view n)
            {
                return entry.name < n;
            });

        if( it == gSolverRegistry.end() || it->name != name)
            return std::unexpected{ "Unknown solver: " + std::string{ name}};

        return it->creator( logger);
    }

    auto SolverFactory::solverNames() const -> std::vector<std::string>
    {
        std::vector<std::string> names;
        names.reserve( gSolverRegistry.size());

        for( const auto& entry : gSolverRegistry)
            names.emplace_back( entry.name);

        return names;
    }
}