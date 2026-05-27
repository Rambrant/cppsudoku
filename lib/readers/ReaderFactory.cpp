//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "ReaderFactory.hpp"

#include "core/Logger.hpp"

namespace com::rambrant::sudoku
{
    auto ReaderFactory::create( std::string_view format,
                                std::istream&    is,
                                const Logger&    logger) const
        -> std::expected<std::unique_ptr<IReader>, std::string>
    {
        const auto* creator = kRegistry.find( format);

        if( ! creator)
        {
            return std::unexpected{ "Unknown reader format: " + std::string{ format}};
        }

        return (*creator)( is, logger);
    }

    auto ReaderFactory::formats() const -> std::vector<std::string>
    {
        return kRegistry.keys();
    }
}