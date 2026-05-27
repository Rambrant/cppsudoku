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
#include "ReaderList.hpp"
#include "factorybase/PluginRegistry.hpp"

namespace com::rambrant::sudoku
{
    class Logger;

    /**
     * @brief Factory that creates @ref IReader instances by format name.
     *
     * The plugin registry is a @c static @c constexpr @ref Registry built at
     * compile time.  The initialiser lambda uses only plugin-type statics
     * (@c T::formatName) and a captureless inner lambda; neither touches any
     * member of @c ReaderFactory, so the class need not be complete.
     *
     * @par Registration
     * Edit @ref ReaderList.hpp only to add a new reader format.
     */
    class ReaderFactory
    {
    public:
        using CreatorFn = std::unique_ptr<IReader>(*)(std::istream&, const Logger&);

        ReaderFactory( const ReaderFactory&)            = delete;
        ReaderFactory& operator=( const ReaderFactory&) = delete;

        static auto instance() -> ReaderFactory&
        {
            static ReaderFactory inst;
            return inst;
        }

        [[nodiscard]]
        auto create( std::string_view format,
                     std::istream&    is,
                     const Logger&    logger) const
            -> std::expected<std::unique_ptr<IReader>, std::string>;

        [[nodiscard]]
        auto formats() const -> std::vector<std::string>;

    private:
        ReaderFactory() = default;

        static constexpr auto kRegistry = makeRegistry<CreatorFn>(
            []<ReaderPlugin T> consteval
            {
                return std::pair<std::string_view, CreatorFn>{
                    T::formatName,
                    +[]( std::istream& is, const Logger& logger) -> std::unique_ptr<IReader>
                    {
                        return std::make_unique<T>( is, logger);
                    }
                };
            },
            std::type_identity<ReaderList>{}
        );
    };
}