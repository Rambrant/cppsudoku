//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <expected>
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
     * @par Registration mechanism (compile-time constexpr array)
     * The registry is a @c constexpr @c std::array of {name, function-pointer}
     * pairs, built and sorted at compile time from @ref ReaderList.
     * It lives in @c .rodata — no heap allocation, zero startup cost.
     *
     * @par Adding a new reader
     * Edit @ref ReaderList.hpp only.
     *
     * @par P2996 migration path
     * When Clang supports C++26 static reflection, @ref ReaderList.hpp is
     * deleted entirely. The public interface of this class is unchanged.
     * See @ref ReaderFactory.cpp for the exact diff.
     */
    class ReaderFactory
    {
    public:

        ReaderFactory( const ReaderFactory&)            = delete;
        ReaderFactory& operator=( const ReaderFactory&) = delete;

        /**
         * @brief Returns the singleton instance.
         */
        static auto instance() -> const ReaderFactory&;

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
         */
        [[nodiscard]]
        auto formats() const -> std::vector<std::string>;

    private:

        ReaderFactory() = default;
    };
}