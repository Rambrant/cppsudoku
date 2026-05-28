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

            SolverFactory( const SolverFactory&)            = delete;
            SolverFactory& operator=( const SolverFactory&) = delete;

            static auto instance() -> SolverFactory&;

            [[nodiscard]]
            auto create( std::string_view format,
                         const Logger&    logger) const
                -> std::expected<std::unique_ptr<ISolver>, std::string>
            {
                return mRegistry.create( format, logger);
            }

            [[nodiscard]]
            auto formats() const -> std::vector<std::string>
            {
                return mRegistry.keys();
            }

            [[nodiscard]]
            auto solverNames() const -> std::vector<std::string>;

        private:

            SolverFactory() = default;   // ← sEntries already in .rodata, nothing to do

            PluginRegistry<ISolver, SolverList> mRegistry;
    };
}