//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "WriterFactory.hpp"

#include <algorithm>

#include "core/Logger.hpp"
#include "WriterList.hpp"   // ← delete this line when migrating to P2996

namespace com::rambrant::sudoku
{
    namespace
    {
        using CreatorFn = std::unique_ptr<IWriter>(*)( std::ostream&, const Logger&);

        struct RegistryEntry
        {
            std::string_view name;
            CreatorFn        creator;
        };

        //
        // P2996 MIGRATION
        //
        // When Clang supports C++26 static reflection (P2996 + P1306):
        //
        // Step 1 — delete the #include "WriterList.hpp" at the top of this file.
        //
        // Step 2 — replace buildRegistry, buildRegistryFromTuple and gWriterRegistry with:
        //
        //   consteval auto buildRegistry()
        //   {
        //       constexpr std::size_t count = []
        //       {
        //           std::size_t n = 0;
        //           for ( auto r : std::meta::members_of( ^com::rambrant::sudoku))
        //               if ( std::meta::is_type( r) && WriterPlugin<[:r:]>) ++n;
        //           return n;
        //       }();
        //
        //       std::array<RegistryEntry, count> entries{};
        //       std::size_t i = 0;
        //       template for ( constexpr auto r : std::meta::members_of( ^com::rambrant::sudoku))
        //       {
        //           if constexpr ( WriterPlugin<[:r:]>)
        //               entries[i++] = { [:r:]::formatName,
        //                                []( std::ostream& os, const Logger& l) -> std::unique_ptr<IWriter>
        //                                { return std::make_unique<[:r:]>( os, l); } };
        //       }
        //       std::sort( entries.begin(), entries.end(),
        //           []( const auto& a, const auto& b) { return a.name < b.name; });
        //       return entries;
        //   }
        //
        //   constexpr auto gWriterRegistry = buildRegistry();
        //
        template<WriterPlugin... Ts>
        constexpr auto buildRegistry() -> std::array<RegistryEntry, sizeof...(Ts)>
        {
            std::array<RegistryEntry, sizeof...(Ts)> entries{{
                RegistryEntry{
                    Ts::formatName,
                    []( std::ostream& os, const Logger& logger) -> std::unique_ptr<IWriter>
                    {
                        return std::make_unique<Ts>( os, logger);
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

        constexpr auto gWriterRegistry = buildRegistryFromTuple(
            std::type_identity<WriterList>{});

    } // anonymous namespace

    auto WriterFactory::instance() -> const WriterFactory&
    {
        static const WriterFactory instance;
        return instance;
    }

    auto WriterFactory::create( std::string_view format,
                                std::ostream&    os,
                                const Logger&    logger) const
        -> std::expected<std::unique_ptr<IWriter>, std::string>
    {
        const auto it = std::lower_bound(
            gWriterRegistry.begin(),
            gWriterRegistry.end(),
            format,
            []( const RegistryEntry& entry, std::string_view n)
            {
                return entry.name < n;
            });

        if( it == gWriterRegistry.end() || it->name != format)
            return std::unexpected{ "Unknown writer format: " + std::string{ format}};

        return it->creator( os, logger);
    }

    auto WriterFactory::formats() const -> std::vector<std::string>
    {
        std::vector<std::string> result;
        result.reserve( gWriterRegistry.size());

        for( const auto& entry : gWriterRegistry)
            result.emplace_back( entry.name);

        return result;
    }
}