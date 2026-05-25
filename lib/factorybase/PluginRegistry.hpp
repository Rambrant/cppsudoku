//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <flat_map>
#include <ranges>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace com::rambrant::sudoku
{
    /**
     * @brief CRTP base that provides singleton access, a keyed registry, and fold-based
     *        plugin registration for ReaderFactory, WriterFactory, and SolverFactory.
     *
     * ### Derived-class contract
     * Each concrete factory must:
     *  - Inherit `PluginRegistry<Derived, CreatorFn>`.
     *  - Declare `friend class PluginRegistry<Derived, CreatorFn>` so the base can call
     *    the private constructor and private @c makeCreator.
     *  - Provide `template<ConceptT T> static auto makeCreator() -> CreatorFn` (private).
     *  - Call `populate(std::type_identity<TypeList>{}, []<typename T>{ return T::key; })`
     *    from its constructor to fill the registry.
     *  - Expose a public `create(...)` that delegates to `findCreator()`.
     *
     * @tparam Derived    The concrete factory type (CRTP pattern).
     * @tparam CreatorFn  The callable type stored in the registry; its exact
     *                    signature is factory-specific.
     */
    template<typename Derived, typename CreatorFn>
    class PluginRegistry
    {
    public:
        PluginRegistry( const PluginRegistry&)            = delete;
        PluginRegistry& operator=( const PluginRegistry&) = delete;

        /**
         * @brief Returns the Meyers-singleton instance.
         *
         * The registry is fully populated on the first call; subsequent calls
         * are a plain static-local access.
         */
        static auto instance() -> Derived&
        {
            static Derived inst;
            return inst;
        }

        /**
         * @brief Sorted list of all registered plugin keys.
         *
         * The order is deterministic because @c std::flat_map keeps keys sorted.
         * Concrete factories expose this under their own name (e.g. @c formats(),
         * @c solverNames()) by delegating here.
         */
        [[nodiscard]]
        auto registeredKeys() const -> std::vector<std::string>
        {
            return std::ranges::to<std::vector>( std::views::keys( mRegistry));
        }

    protected:
        PluginRegistry() = default;

        /**
         * @brief Registers every plugin type in @p TypeList.
         *
         * @tparam TypeList  A @c std::tuple of plugin types.
         * @tparam KeyOf     A generic lambda `[]<typename T>{ return T::keyMember; }`
         *                   invoked once per plugin type to obtain its registry key.
         *
         * Calls `Derived::makeCreator<T>()` for each type, so the concrete factory
         * decides how each plugin is constructed.
         */
        template<typename TypeList, typename KeyOf>
        void populate( std::type_identity<TypeList>, KeyOf keyOf)
        {
            [this, keyOf]<typename... Ts>( std::type_identity<std::tuple<Ts...>>)
            {
                ( mRegistry.emplace(
                    std::string{ keyOf.template operator()<Ts>()},
                    Derived::template makeCreator<Ts>()
                ), ...);
            }( std::type_identity<TypeList>{});
        }

        /**
         * @brief Looks up the creator for a given key.
         *
         * @return A pointer to the @c CreatorFn if found, @c nullptr otherwise.
         *         The pointer is valid for the lifetime of this registry.
         */
        [[nodiscard]]
        auto findCreator( std::string_view key) const -> const CreatorFn*
        {
            const auto it = mRegistry.find( std::string{ key});
            return it != mRegistry.end() ? &it->second : nullptr;
        }

        std::flat_map<std::string, CreatorFn> mRegistry;
    };
}