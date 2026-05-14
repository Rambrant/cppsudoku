//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "SolverFactory.hpp"

#include <ranges>

#include "core/Logger.hpp"
#include "SolverList.hpp"   // ← delete this line when migrating to P2996

namespace com::rambrant::sudoku
{
    //
    //  Creator helper
    //
    //
    // makeCreator<T>() returns the CreatorFn for a concrete solver type T.
    // Constrained on SolverPlugin so a type without a valid solverName is
    // rejected at the call site.  This helper is the one piece of code that
    // does NOT change during the P2996 migration.
    //
    template<SolverPlugin T>
    static auto makeCreator() -> SolverFactory::CreatorFn
    {
        return []( const Logger& logger) -> std::unique_ptr<ISolver>
        {
            return std::make_unique<T>( logger);
        };
    }

    //
    //  Constructor — current implementation (compile-time type list)
    //
    //  P2996 MIGRATION
    //
    //  When Clang supports C++26 static reflection (P2996 + P1306):
    //
    //  Step 1 — delete the #include "SolverList.hpp" at the top of
    //           this file.
    //
    //  Step 2 — replace this entire constructor body with:
    //
    //    consteval auto discoverSolverPlugins()
    //    {
    //        std::vector<std::meta::info> found;
    //        for ( auto r : std::meta::members_of( ^com::rambrant::sudoku))
    //            if ( std::meta::is_type( r) && SolverPlugin<[:r:]>)
    //                found.push_back( r);
    //        return found;
    //    }
    //
    //    SolverFactory::SolverFactory()
    //    {
    //        template for ( constexpr auto r : discoverSolverPlugins())
    //        {
    //            mRegistry.emplace( std::string{ [:r:]::solverName},
    //                               makeCreator<[:r:]>());
    //        }
    //    }
    //
    //  makeCreator<T>() above is unchanged.  SolverList.hpp is deleted.
    //  Everything else in this project is unchanged.
    //
    SolverFactory::SolverFactory()
    {
        [this]<typename... Ts>( std::type_identity<std::tuple<Ts...>>)
        {
            ( mRegistry.emplace( std::string{ Ts::solverName}, makeCreator<Ts>()), ...);
        }( std::type_identity<SolverList>{});
    }

    //
    //  Singleton, create, solverNames
    //
    auto SolverFactory::instance() -> SolverFactory&
    {
        static SolverFactory factory;
        return factory;
    }

    auto SolverFactory::create( std::string_view name,
                                const Logger&    logger) const
        -> std::expected<std::unique_ptr<ISolver>, std::string>
    {
        const auto it = mRegistry.find( std::string{ name});

        if( it == mRegistry.end())
        {
            return std::unexpected{ "Unknown solver: " + std::string{ name}};
        }

        return it->second( logger);
    }

    auto SolverFactory::solverNames() const -> std::vector<std::string>
    {
        return std::ranges::to<std::vector>( std::views::keys( mRegistry));
    }

}