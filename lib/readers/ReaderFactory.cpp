//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "ReaderFactory.hpp"

#include <algorithm>

#include "core/Logger.hpp"
#include "ReaderList.hpp"   // ← delete this line when migrating to P2996

namespace com::rambrant::sudoku
{
    namespace
    {
        using CreatorFn = std::unique_ptr<IReader>(*)( std::istream&, const Logger&);

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
        // Step 1 — delete the #include "ReaderList.hpp" at the top of this file.
        //
        // Step 2 — replace buildRegistry, buildRegistryFromTuple and gReaderRegistry with:
        //
        //   consteval auto buildRegistry()
        //   {
        //       constexpr std::size_t count = []
        //       {
        //           std::size_t n = 0;
        //           for ( auto r : std::meta::members_of( ^com::rambrant::sudoku))
        //               if ( std::meta::is_type( r) && ReaderPlugin<[:r:]>) ++n;
        //           return n;
        //       }();
        //
        //       std::array<RegistryEntry, count> entries{};
        //       std::size_t i = 0;
        //       template for ( constexpr auto r : std::meta::members_of( ^com::rambrant::sudoku))
        //       {
        //           if constexpr ( ReaderPlugin<[:r:]>)
        //               entries[i++] = { [:r:]::formatName,
        //                                []( std::istream& is, const Logger& l) -> std::unique_ptr<IReader>
        //                                { return std::make_unique<[:r:]>( is, l); } };
        //       }
        //       std::sort( entries.begin(), entries.end(),
        //           []( const auto& a, const auto& b) { return a.name < b.name; });
        //       return entries;
        //   }
        //
        //   constexpr auto gReaderRegistry = buildRegistry();
        //
        template<ReaderPlugin... Ts>
        constexpr auto buildRegistry() -> std::array<RegistryEntry, sizeof...(Ts)>
        {
            std::array<RegistryEntry, sizeof...(Ts)> entries{{
                RegistryEntry{
                    Ts::formatName,
                    []( std::istream& is, const Logger& logger) -> std::unique_ptr<IReader>
                    {
                        return std::make_unique<Ts>( is, logger);
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

        constexpr auto gReaderRegistry = buildRegistryFromTuple(
            std::type_identity<ReaderList>{});

    } // anonymous namespace

    auto ReaderFactory::instance() -> const ReaderFactory&
    {
        static const ReaderFactory instance;
        return instance;
    }

    auto ReaderFactory::create( std::string_view format,
                                std::istream&    is,
                                const Logger&    logger) const
        -> std::expected<std::unique_ptr<IReader>, std::string>
    {
        const auto it = std::lower_bound(
            gReaderRegistry.begin(),
            gReaderRegistry.end(),
            format,
            []( const RegistryEntry& entry, std::string_view n)
            {
                return entry.name < n;
            });

        if( it == gReaderRegistry.end() || it->name != format)
            return std::unexpected{ "Unknown reader format: " + std::string{ format}};

        return it->creator( is, logger);
    }

    auto ReaderFactory::formats() const -> std::vector<std::string>
    {
        std::vector<std::string> result;
        result.reserve( gReaderRegistry.size());

        for( const auto& entry : gReaderRegistry)
            result.emplace_back( entry.name);

        return result;
    }
}