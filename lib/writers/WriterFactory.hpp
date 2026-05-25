//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <expected>
#include <functional>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#include "IWriter.hpp"
#include "factorybase//PluginRegistry.hpp"

namespace com::rambrant::sudoku
{
    class Logger;

    /**
     * @brief Factory that creates @ref IWriter instances by format name.
     *
     * Mirrors @ref ReaderFactory exactly — inherits singleton access, registry
     * storage, and fold-based registration from @ref PluginRegistry.
     * See @ref ReaderFactory for the registration mechanism and P2996 migration path.
     */
    class WriterFactory : public PluginRegistry<WriterFactory,
                              std::function<std::unique_ptr<IWriter>( std::ostream&, const Logger&)>>
    {
    public:
        using CreatorFn = std::function<std::unique_ptr<IWriter>( std::ostream&, const Logger&)>;

        /**
         * @brief Constructs the @ref IWriter for the given format name.
         *
         * @param format  Key as supplied on the command line (e.g. @c "block").
         * @param os      Output stream forwarded verbatim to the concrete writer.
         * @param logger  Logger forwarded verbatim to the concrete writer.
         * @return The writer on success, or an error message if @p format is unknown.
         */
        [[nodiscard]]
        auto create( std::string_view format,
                     std::ostream&    os,
                     const Logger&    logger) const
            -> std::expected<std::unique_ptr<IWriter>, std::string>;

        /**
         * @brief Sorted list of every registered format key.
         *
         * Feed this to @ref ValuesIn to keep command-line validation in sync
         * with @ref WriterList automatically.
         */
        [[nodiscard]]
        auto formats() const -> std::vector<std::string>;

    private:
        WriterFactory();
        friend class PluginRegistry<WriterFactory, CreatorFn>;

        template<WriterPlugin T>
        static auto makeCreator() -> CreatorFn;
    };
}