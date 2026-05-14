//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "ReaderFactory.hpp"

#include <ranges>

#include "core/Logger.hpp"
#include "ReaderList.hpp"   // ← delete this line when migrating to P2996

namespace com::rambrant::sudoku
{
    //
    //  Creator helper
    //
    // makeCreator<T>() returns the CreatorFn for a concrete reader type T.
    // It is templated on a ReaderPlugin so the concept constraint is enforced
    // at the call site.  Both the current fold-based constructor and the future
    // P2996-based constructor call this helper identically — it is the one
    // piece of code that does NOT change during the migration.
    //
    template<ReaderPlugin T>
    static auto makeCreator() -> ReaderFactory::CreatorFn
    {
        return []( std::istream& is, const Logger& logger) -> std::unique_ptr<IReader>
        {
            return std::make_unique<T>( is, logger);
        };
    }

    //
    //  Constructor — current implementation (compile-time type list)
    //
    // The registry is built by folding over ReaderList (a std::tuple of all
    // ReaderPlugin types).  std::type_identity<ReaderList>{} passes the tuple
    // as a type rather than a value, which avoids constructing any readers at
    // registration time.  The template lambda unpacks it into Ts..., and the
    // comma fold visits each type exactly once.
    //
    //  P2996 MIGRATION
    //
    //   When Clang supports C++26 static reflection (P2996 + P1306):
    //
    //   Step 1 — delete the #include "ReaderList.hpp" at the top of
    //            this file.
    //
    //   Step 2 — replace this entire constructor body with:
    //
    //     consteval auto discoverReaderPlugins()
    //     {
    //         std::vector<std::meta::info> found;
    //         for ( auto r : std::meta::members_of( ^com::rambrant::sudoku))
    //             if ( std::meta::is_type( r) && ReaderPlugin<[:r:]>)
    //                 found.push_back( r);
    //         return found;
    //     }
    //
    //     ReaderFactory::ReaderFactory()
    //     {
    //         template for ( constexpr auto r : discoverReaderPlugins())
    //         {
    //             mRegistry.emplace( std::string{ [:r:]::formatName},
    //                                makeCreator<[:r:]>());
    //         }
    //     }
    //
    //   makeCreator<T>() above is unchanged.  ReaderList.hpp is deleted.
    //   Everything else in this project is unchanged.
    //
    ReaderFactory::ReaderFactory()
    {
        [this]<typename... Ts>( std::type_identity<std::tuple<Ts...>>)
        {
            ( mRegistry.emplace( std::string{ Ts::formatName}, makeCreator<Ts>()), ...);
        }( std::type_identity<ReaderList>{});
    }

    //
    //  Singleton, create, formats
    //
    auto ReaderFactory::instance() -> ReaderFactory&
    {
        static ReaderFactory factory;

        return factory;
    }

    auto ReaderFactory::create( std::string_view format,
                                std::istream&    is,
                                const Logger&    logger) const
        -> std::expected<std::unique_ptr<IReader>, std::string>
    {
        const auto it = mRegistry.find( std::string{ format});

        if( it == mRegistry.end())
        {
            return std::unexpected{ "Unknown reader format: " + std::string{ format}};
        }

        return it->second( is, logger);
    }

    auto ReaderFactory::formats() const -> std::vector<std::string>
    {
        return std::ranges::to<std::vector>( std::views::keys( mRegistry));
    }

}