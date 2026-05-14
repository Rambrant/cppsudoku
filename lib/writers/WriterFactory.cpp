//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "WriterFactory.hpp"

#include <ranges>

#include "core/Logger.hpp"
#include "WriterList.hpp"   // ← delete this line when migrating to P2996

namespace com::rambrant::sudoku
{
    //
    //  Creator helper
    //
    //
    // See ReaderFactory.cpp for full commentary.  makeCreator<T>() is the one
    // piece of code that does NOT change during the P2996 migration.
    //
    template<WriterPlugin T>
    static auto makeCreator() -> WriterFactory::CreatorFn
    {
        return []( std::ostream& os, const Logger& logger) -> std::unique_ptr<IWriter>
        {
            return std::make_unique<T>( os, logger);
        };
    }

    //
    //  Constructor — current implementation (compile-time type list)
    //
    //  P2996 MIGRATION
    //
    //  Step 1 — delete the #include "WriterList.hpp" at the top.
    //
    //  Step 2 — replace this constructor body with:
    //
    //    consteval auto discoverWriterPlugins()
    //    {
    //        std::vector<std::meta::info> found;
    //        for ( auto r : std::meta::members_of( ^com::rambrant::sudoku))
    //            if ( std::meta::is_type( r) && WriterPlugin<[:r:]>)
    //                found.push_back( r);
    //        return found;
    //    }
    //
    //    WriterFactory::WriterFactory()
    //    {
    //        template for ( constexpr auto r : discoverWriterPlugins())
    //        {
    //            mRegistry.emplace( std::string{ [:r:]::formatName},
    //                               makeCreator<[:r:]>());
    //        }
    //    }
    //
    //  makeCreator<T>() above is unchanged.  WriterList.hpp is deleted.
    //  Everything else in this project is unchanged.
    //
    WriterFactory::WriterFactory()
    {
        [this]<typename... Ts>( std::type_identity<std::tuple<Ts...>>)
        {
            ( mRegistry.emplace( std::string{ Ts::formatName}, makeCreator<Ts>()), ...);
        }( std::type_identity<WriterList>{});
    }

    //
    //  Singleton, create, formats
    //
    auto WriterFactory::instance() -> WriterFactory&
    {
        static WriterFactory factory;
        return factory;
    }

    auto WriterFactory::create( std::string_view format,
                                std::ostream&    os,
                                const Logger&    logger) const
        -> std::expected<std::unique_ptr<IWriter>, std::string>
    {
        const auto it = mRegistry.find( std::string{ format});

        if( it == mRegistry.end())
        {
            return std::unexpected{ "Unknown writer format: " + std::string{ format}};
        }

        return it->second( os, logger);
    }

    auto WriterFactory::formats() const -> std::vector<std::string>
    {
        return std::ranges::to<std::vector>( std::views::keys( mRegistry));
    }

}