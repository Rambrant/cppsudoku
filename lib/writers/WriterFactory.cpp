//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "WriterFactory.hpp"

#include "core/Logger.hpp"

namespace com::rambrant::sudoku
{
    auto WriterFactory::create( std::string_view format,
                                std::ostream&    os,
                                const Logger&    logger) const
        -> std::expected<std::unique_ptr<IWriter>, std::string>
    {
        const auto* creator = kRegistry.find( format);

        if( ! creator)
        {
            return std::unexpected{ "Unknown writer format: " + std::string{ format}};
        }

        return (*creator)( os, logger);
    }

    auto WriterFactory::formats() const -> std::vector<std::string>
    {
        return kRegistry.keys();
    }
}