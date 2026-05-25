//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <expected>
#include <functional>
#include <istream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "IReader.hpp"
#include "factorybase//PluginRegistry.hpp"

namespace com::rambrant::sudoku
{
    class Logger;

    /**
     * @brief Factory that creates @ref IReader instances by format name.
     *
     * Inherits singleton access, registry storage, and fold-based registration
     * from @ref PluginRegistry.
     *
     * @par Registration mechanism (current: compile-time type list)
     * The registry is populated in the constructor by folding over
     * @ref ReaderList.  Edit @ref ReaderList.hpp only to add a new format.
     *
     * @par P2996 migration path
     * When Clang supports C++26 static reflection, @ref ReaderList.hpp is
     * deleted.  The constructor discovers all @ref ReaderPlugin types in the
     * namespace automatically.  The public interface is unchanged.
     */
    class ReaderFactory : public PluginRegistry<ReaderFactory,
                              std::function<std::unique_ptr<IReader>( std::istream&, const Logger&)>>
    {
    public:
        using CreatorFn = std::function<std::unique_ptr<IReader>( std::istream&, const Logger&)>;

        /**
         * @brief Constructs the @ref IReader for the given format name.
         *
         * @param format  Key as supplied on the command line (e.g. @c "text").
         * @param is      Input stream forwarded verbatim to the concrete reader.
         * @param logger  Logger forwarded verbatim to the concrete reader.
         * @return The reader on success, or an error message if @p format is unknown.
         */
        [[nodiscard]]
        auto create( std::string_view format,
                     std::istream&    is,
                     const Logger&    logger) const
            -> std::expected<std::unique_ptr<IReader>, std::string>;

        /**
         * @brief Sorted list of every registered format key.
         *
         * Feed this to @ref ValuesIn to keep command-line validation in sync
         * with @ref ReaderList automatically.
         */
        [[nodiscard]]
        auto formats() const -> std::vector<std::string>;

    private:
        ReaderFactory();
        friend class PluginRegistry<ReaderFactory, CreatorFn>;

        template<ReaderPlugin T>
        static auto makeCreator() -> CreatorFn;
    };
}