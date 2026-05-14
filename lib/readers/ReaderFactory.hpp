//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <expected>
#include <flat_map>
#include <functional>
#include <istream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "IReader.hpp"

namespace com::rambrant::sudoku
{
    class Logger;

    /**
     * @brief Factory that creates @ref IReader instances by format name.
     *
     * @par Registration mechanism (current: compile-time type list)
     * The registry is populated in the constructor by folding over
     * @ref ReaderList, a @c std::tuple of all @ref ReaderPlugin types.
     * Each type contributes its @c formatName key and a constructor wrapper.
     * No static initialisers and no @c WHOLE_ARCHIVE linker flag are involved.
     *
     * @par Adding a new reader
     * Edit @ref ReaderList.hpp only — include the new header and append the
     * type to @c ReaderList.  Nothing else needs to change.
     *
     * @par P2996 migration path
     * When Clang supports C++26 static reflection, @ref ReaderList.hpp is
     * deleted entirely.  The constructor discovers all @ref ReaderPlugin types
     * in the @c com::rambrant::sudoku namespace automatically.  The public
     * interface of this class is unchanged.  See @ref ReaderFactory.cpp for
     * the exact diff.
     */
    class ReaderFactory
    {
    public:

        using CreatorFn = std::function<std::unique_ptr<IReader>( std::istream&, const Logger&)>;

        ReaderFactory( const ReaderFactory&)            = delete;
        ReaderFactory& operator=( const ReaderFactory&) = delete;

        /**
         * @brief Returns the Meyer's-singleton instance.
         *
         * The registry is fully populated on first call; every subsequent call
         * is a plain @c static local access.
         */
        static auto instance() -> ReaderFactory&;

        /**
         * @brief Constructs the @ref IReader for the given format name.
         *
         * @param format  Key as supplied on the command line (e.g. @c "text").
         * @param is      Input stream forwarded verbatim to the concrete reader.
         * @param logger  Logger forwarded verbatim to the concrete reader.
         * @return The reader on success, or an error message if @p format is
         *         unknown.
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
         * with @ref ReaderList automatically:
         * @code
         *   inFormatOpt.setValidator(
         *       ValuesIn( ReaderFactory::instance().formats()));
         * @endcode
         */
        [[nodiscard]]
        auto formats() const -> std::vector<std::string>;

    private:

        ReaderFactory();

        std::flat_map<std::string, CreatorFn> mRegistry;
    };

}