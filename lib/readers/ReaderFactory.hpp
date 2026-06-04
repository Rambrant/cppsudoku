#pragma once

#include <expected>
#include <istream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "IReader.hpp"
#include "ReaderList.hpp"         // needed — TypeList must be visible here
#include "core/PluginRegistry.hpp"

namespace com::rambrant::sudoku
{
    class Logger;

    /**
     * @brief Singleton facade over the compile-time @ref PluginRegistry for readers.
     *
     * All registry data lives in @c PluginRegistry::sEntries — a
     * @c constexpr array in @c .rodata.  This class has no instance data;
     * its sole purpose is to expose the familiar factory API.
     *
     * @par Adding a reader
     * Edit @ref ReaderList.hpp only.
     *
     * @par P2996 migration
     * Delete @ref ReaderList.hpp and remove the @p TypeList parameter from the
     * @ref PluginRegistry instantiation.  Nothing else changes.
     */
    class ReaderFactory
    {
        public:

            ReaderFactory( const ReaderFactory&)            = delete;
            ReaderFactory& operator=( const ReaderFactory&) = delete;

            static auto instance() -> ReaderFactory&;

            /**
             * @brief Creates the plugin identified by @p key.
             *
             * Delegates to @ref ReaderFactory
             *
             * @return The plugin on success; an error string if @p key is unknown.
             */
            [[nodiscard]]
            auto create( std::string_view format,
                         std::istream&    is,
                         const Logger&    logger) const
                -> std::expected<std::unique_ptr<IReader>, std::string>
            {
                return mRegistry.create( format, is, logger);
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

            ReaderFactory() = default;

            PluginRegistry<IReader, ReaderList, std::istream&> mRegistry;
    };
}