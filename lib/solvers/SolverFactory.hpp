//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <expected>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "ISolver.hpp"
#include "SolverList.hpp"
#include "factorybase/PluginRegistry.hpp"

namespace com::rambrant::sudoku
{
    class Logger;

    /**
     * @brief Factory that creates @ref ISolver instances by solver name.
     *
     * @note Unlike @ref ReaderFactory and @ref WriterFactory, the @c CreatorFn
     *       takes only a @ref Logger — no stream.
     */
    class SolverFactory
    {
    public:
        using CreatorFn = std::unique_ptr<ISolver>(*)(const Logger&);

        SolverFactory( const SolverFactory&)            = delete;
        SolverFactory& operator=( const SolverFactory&) = delete;

        static auto instance() -> SolverFactory&
        {
            static SolverFactory inst;
            return inst;
        }

        [[nodiscard]]
        auto create( std::string_view name,
                     const Logger&    logger) const
            -> std::expected<std::unique_ptr<ISolver>, std::string>;

        /**
         * @brief Sorted list of every registered solver name.
         *
         * Feed this to @ref ValuesIn to keep command-line validation in sync
         * with @ref SolverList automatically.
         */
        [[nodiscard]]
        auto solverNames() const -> std::vector<std::string>;

    private:
        SolverFactory() = default;

        static constexpr auto kRegistry = makeRegistry<CreatorFn>(
            []<SolverPlugin T> consteval
            {
                return std::pair<std::string_view, CreatorFn>{
                    T::solverName,
                    +[]( const Logger& logger) -> std::unique_ptr<ISolver>
                    {
                        return std::make_unique<T>( logger);
                    }
                };
            },
            std::type_identity<SolverList>{}
        );
    };
}