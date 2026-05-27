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
            using CreatorFn = std::unique_ptr<IWriter>(*)(std::ostream&, const Logger&);

            WriterFactory( const WriterFactory&)            = delete;
            WriterFactory& operator=( const WriterFactory&) = delete;

            static auto instance() -> WriterFactory&
            {
                static WriterFactory inst;
                return inst;
            }

            [[nodiscard]]
            auto create( std::string_view format,
                         std::ostream&    os,
                         const Logger&    logger) const
                -> std::expected<std::unique_ptr<IWriter>, std::string>;

            [[nodiscard]]
            auto formats() const -> std::vector<std::string>;

        private:
            WriterFactory() = default;

            static constexpr auto kRegistry = makeRegistry<CreatorFn>(
                []<WriterPlugin T> consteval
                {
                    return std::pair<std::string_view, CreatorFn>{
                        T::formatName,
                        +[]( std::ostream& os, const Logger& logger) -> std::unique_ptr<IWriter>
                        {
                            return std::make_unique<T>( os, logger);
                        }
                    };
                },
                std::type_identity<WriterList>{}
            );
    };
}