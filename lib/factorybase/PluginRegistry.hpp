//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <algorithm>
#include <array>
#include <expected>
#include <memory>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace com::rambrant::sudoku
{
    class Logger;

    /**
     * @brief Unified compile-time concept for every plugin type.
     *
     * A type satisfies @c PluginType<Base> when it:
     *   - publicly derives from @p Base, and
     *   - exposes @c static constexpr std::string_view entityName.
     *
     * @tparam T    The concrete plugin class.
     * @tparam Base The abstract base it must derive from.
     */
    template<typename T, typename Base>
    concept PluginType =
        std::derived_from<T, Base> &&
        requires { { T::entityName } -> std::convertible_to<std::string_view>; };

    /**
     * @brief Compile-time plugin registry mapping string keys to factory functions.
     *
     * All entries are built into a @c constexpr @c std::array at compile time,
     * sorted by key for O(log N) binary search at runtime.
     * There is zero runtime initialisation cost.
     *
     * @tparam Base     Abstract base class; plugins must derive from it.
     * @tparam TypeList A @c std::tuple of every @ref PluginType<Base> to register.
     * @tparam Args     Constructor arguments that precede the @ref Logger
     *                  (e.g. @c std::istream& for readers, @c std::ostream& for writers;
     *                  empty pack for solvers).
     *
     * @par Constructor convention
     * Every registered type @c T must be constructible as @c T(Args..., const Logger&).
     *
     * @par P2996 migration
     * Replace the @p TypeList template parameter and @c buildEntries() with a
     * @c consteval discovery loop over @c std::meta::members_of when Clang
     * supports it.  The public interface and all three factory classes are unchanged.
     */
    template<typename Base, typename TypeList, typename... Args>
    class PluginRegistry
    {
    public:

        /// @brief Raw function-pointer type for a plugin constructor wrapper.
        using CreatorFn = std::unique_ptr<Base>(*)( Args..., const Logger&);

        /**
         * @brief Creates the plugin identified by @p key.
         *
         * Uses @c std::ranges::lower_bound on the compile-time sorted table.
         *
         * @return The plugin on success; an error string if @p key is unknown.
         */
        [[nodiscard]]
        auto create( std::string_view key, Args... args, const Logger& logger) const
            -> std::expected<std::unique_ptr<Base>, std::string>
        {
            const auto it = std::ranges::find( sEntries, key, &Entry::key);

            if( it == sEntries.end())
            {
                return std::unexpected{ "Unknown plugin: " + std::string{ key}};
            }

            return it->creator( std::forward<Args>( args)..., logger);        }

        /**
         * @brief Sorted list of all registered keys.
         * Feed this to @ref ValuesIn to keep command-line validation in sync automatically.
         */
        [[nodiscard]]
        auto keys() const -> std::vector<std::string>
        {
            std::vector<std::string> result;
            result.reserve( SIZE);

            for( const auto& [key, _] : sEntries)
            {
                result.emplace_back( key);
            }

            return result;
        }

    private:

        struct Entry
        {
            std::string_view key;
            CreatorFn        creator;
        };

        static constexpr std::size_t SIZE = std::tuple_size_v<std::remove_cvref_t<TypeList>>;

        //
        //  Returns a constexpr function pointer to T's constructor wrapper.
        //  Non-capturing lambdas convert to raw function pointers constexpr-safely (C++17).
        //
        template<PluginType<Base> T>
        static constexpr auto makeCreatorFn() -> CreatorFn
        {
            return []( Args... args, const Logger& logger) -> std::unique_ptr<Base>
            {
                return std::make_unique<T>( std::forward<Args>( args)..., logger);
            };
        }

        //
        //  Builds and sorts the entry table entirely at compile time.
        //  std::ranges::sort is constexpr since C++20.
        //
            static constexpr auto buildEntries() -> std::array<Entry, SIZE>
        {
            auto arr = []<typename... Ts>( std::type_identity<std::tuple<Ts...>>)
                -> std::array<Entry, sizeof...( Ts)>
            {
                return { Entry{ Ts::entityName, makeCreatorFn<Ts>() }... };
            }( std::type_identity<std::remove_cvref_t<TypeList>>{});

            std::ranges::sort( arr, {}, &Entry::key);

            return arr;
        }

        //
        //  The entire registry lives here — in .rodata, zero runtime cost.
        //
        static constexpr std::array<Entry, SIZE> sEntries = buildEntries();
    };
}