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

namespace com::rambrant::sudoku
{
    class Logger;

    /**
     * @brief Factory that creates @ref IWriter instances by format name.
     *
     * @par Registration mechanism (compile-time constexpr array)
     * The registry is a @c constexpr @c std::array of {name, function-pointer}
     * pairs, built and sorted at compile time from @ref WriterList.
     * It lives in @c .rodata — no heap allocation, zero startup cost.
     *
     * @par Adding a new writer
     * Edit @ref WriterList.hpp only.
     *
     * @par P2996 migration path
     * When Clang supports C++26 static reflection, @ref WriterList.hpp is
     * deleted entirely. The public interface of this class is unchanged.
     * See @ref WriterFactory.cpp for the exact diff.
     */
    class WriterFactory
    {
    public:

        WriterFactory( const WriterFactory&)            = delete;
        WriterFactory& operator=( const WriterFactory&) = delete;

        /**
         * @brief Returns the singleton instance.
         */
        static auto instance() -> const WriterFactory&;

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
         */
        [[nodiscard]]
        auto formats() const -> std::vector<std::string>;

    private:

        WriterFactory() = default;
    };
}