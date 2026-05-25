//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "ReaderFactory.hpp"

#include "core/Logger.hpp"
#include "ReaderList.hpp"   // ← delete this line when migrating to P2996

namespace com::rambrant::sudoku
{
    //
    //  makeCreator<T> — private static, called by PluginRegistry::populate()
    //
    //  Constrained on ReaderPlugin so a type without a valid formatName is
    //  rejected at the call site.
    //
    template<ReaderPlugin T>
    auto ReaderFactory::makeCreator() -> ReaderFactory::CreatorFn
    {
        return []( std::istream& is, const Logger& logger) -> std::unique_ptr<IReader>
        {
            return std::make_unique<T>( is, logger);
        };
    }

    //
    //  Constructor — registers every type in ReaderList via the base populate()
    //
    //  P2996 MIGRATION
    //
    //   Step 1 — delete the #include "ReaderList.hpp" above.
    //
    //   Step 2 — replace this constructor body with:
    //
    //     ReaderFactory::ReaderFactory()
    //     {
    //         consteval auto discoverReaderPlugins()
    //         {
    //             std::vector<std::meta::info> found;
    //             for ( auto r : std::meta::members_of( ^com::rambrant::sudoku))
    //                 if ( std::meta::is_type( r) && ReaderPlugin<[:r:]>)
    //                     found.push_back( r);
    //             return found;
    //         }
    //         template for ( constexpr auto r : discoverReaderPlugins())
    //             mRegistry.emplace( std::string{ [:r:]::formatName}, makeCreator<[:r:]>());
    //     }
    //
    ReaderFactory::ReaderFactory()
    {
        populate(
            std::type_identity<ReaderList>{},
            []<typename T>{ return T::formatName; }
        );
    }

    auto ReaderFactory::create( std::string_view format,
                                std::istream&    is,
                                const Logger&    logger) const
        -> std::expected<std::unique_ptr<IReader>, std::string>
    {
        const auto* creator = findCreator( format);
        if( !creator)
            return std::unexpected{ "Unknown reader format: " + std::string{ format}};
        return (*creator)( is, logger);
    }

    auto ReaderFactory::formats() const -> std::vector<std::string>
    {
        return registeredKeys();
    }
}