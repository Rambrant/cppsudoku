//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <expected>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#include "IWriter.hpp"
#include "WriterList.hpp"
#include "factorybase/PluginRegistry.hpp"

namespace com::rambrant::sudoku
{
    class Logger;

    /**
     * @brief Factory that creates @ref IWriter instances by format name.
     *
     * Mirrors @ref ReaderFactory — see its documentation for the design rationale.
     */
    class WriterFactory
    {
        public:

            WriterFactory( const WriterFactory&)            = delete;
            WriterFactory& operator=( const WriterFactory&) = delete;

            static auto instance() -> WriterFactory&;

            /**
             * @brief Creates the plugin identified by @p key.
             *
             * Uses @c std::ranges::lower_bound on the compile-time sorted table.
             *
             * @return The plugin on success; an error string if @p key is unknown.
             */
            [[nodiscard]]
            auto create( std::string_view format,
                         std::ostream&    os,
                         const Logger&    logger) const
                -> std::expected<std::unique_ptr<IWriter>, std::string>
            {
                return mRegistry.create( format, os, logger);
            }

            /**
             * @brief Return sorted list of formats.
             *            *
             * @return The sorted list of the plugins entityNames
             */
            [[nodiscard]]
            auto formats() const -> std::vector<std::string>
            {
                return mRegistry.keys();
            }

        private:

            WriterFactory() = default;

            PluginRegistry<IWriter, WriterList, std::ostream&> mRegistry;
    };
}