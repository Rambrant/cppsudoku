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
#include "core/PluginRegistry.hpp"

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

            SolverFactory( const SolverFactory&)            = delete;
            SolverFactory& operator=( const SolverFactory&) = delete;

            static auto instance() -> SolverFactory&;

            /**
             * @brief Creates the plugin identified by @p key.
             *
             * Uses @c std::ranges::lower_bound on the compile-time sorted table.
             *
             * @return The plugin on success; an error string if @p key is unknown.
             */
            [[nodiscard]]
            auto create( std::string_view format,
                         const Logger&    logger) const
                -> std::expected<std::unique_ptr<ISolver>, std::string>
            {
                return mRegistry.create( format, logger);
            }

            /**
             * @brief Return sorted list of pluginKeys.
             *            *
             * @return The sorted list of the plugins pluginKey string_view
             */
            [[nodiscard]]
            auto pluginKeys() const -> std::vector<std::string>
            {
                return mRegistry.keys();
            }

        private:

            SolverFactory() = default;   // ← sEntries already in .rodata, nothing to do

            PluginRegistry<ISolver, SolverList> mRegistry;
    };
}