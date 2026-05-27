//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <algorithm>
#include <array>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace com::rambrant::sudoku
{
    /**
     * @brief A compile-time sorted lookup table mapping string keys to plugin creators.
     *
     * Stored as a @c static @c constexpr member of each concrete factory.
     * Because the actual factory's @c constexpr initialiser uses only plugin-type statics and
     * captureless lambdas — none of which require the factory to be complete.
     *
     * @tparam CreatorFn  Raw function-pointer type — must be constexpr-constructible.
     * @tparam RegSize    Number of registered plugins, deduced from the entry array.
     */
    template<typename CreatorFn, std::size_t RegSize>
    class PluginRegistry
    {
    public:
        using Entry = std::pair<std::string_view, CreatorFn>;

        /**
         * @brief Constructs the registry, sorting entries by key at compile time.
         */
        constexpr explicit PluginRegistry( std::array<Entry, RegSize> entries) noexcept
            : mEntries( sortedByKey( std::move( entries)))
        {}

        /**
         * @brief Binary-searches for a creator by key.  O(log N) at runtime.
         * @return Pointer to the matching @c CreatorFn, or @c nullptr.
         *         Valid for the lifetime of the process (static storage).
         */
        [[nodiscard]]
        constexpr auto find( std::string_view key) const noexcept -> const CreatorFn*
        {
            const auto it = std::lower_bound(
                mEntries.begin(), mEntries.end(), key,
                []( const Entry& e, std::string_view k) noexcept { return e.first < k; });

            return ( it != mEntries.end() && it->first == key) ? &it->second : nullptr;
        }

        /**
         * @brief Returns all registered keys in sorted order.  O(N).
         */
        [[nodiscard]]
        auto keys() const -> std::vector<std::string>
        {
            std::vector<std::string> result;

            result.reserve( RegSize);

            for( const auto& [key, _] : mEntries)
                result.emplace_back( key);

            return result;
        }

    private:
        std::array<Entry, RegSize> mEntries;

        static consteval auto sortedByKey( std::array<Entry, RegSize> arr) noexcept
            -> std::array<Entry, RegSize>
        {
            std::sort( arr.begin(), arr.end(),
                       []( const Entry& a, const Entry& b) noexcept
                       {
                           return a.first < b.first;
                       });
            return arr;
        }
    };

    /**
     * @brief Builds a @ref Registry from a plugin type-list and a per-type entry factory.
     *
     * The entry factory is a consteval generic lambda:
     * @code
     *   []<ConceptT T> consteval {
     *       return std::pair<std::string_view, CreatorFn>{
     *           T::keyMember,
     *           +[](...) -> ReturnType { return std::make_unique<T>(...); }
     *       };
     *   }
     * @endcode
     * Only plugin-type statics and captureless lambdas appear inside it —
     * no reference to the factory class — so it evaluates while the factory
     * is still incomplete.
     *
     * @tparam CreatorFn  Must be supplied explicitly; cannot be deduced.
     */
    template<typename CreatorFn, typename Fn, typename... Ts>
    consteval auto makeRegistry( Fn makeEntry, std::type_identity<std::tuple<Ts...>>)
        -> PluginRegistry<CreatorFn, sizeof...( Ts)>
    {
        using Entry = std::pair<std::string_view, CreatorFn>;

        return PluginRegistry<CreatorFn, sizeof...( Ts)>{
            std::array<Entry, sizeof...( Ts)>{ makeEntry.template operator()<Ts>()... }
        };
    }
}