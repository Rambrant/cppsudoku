//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "SolverFactory.hpp"

#include "core/Logger.hpp"

namespace com::rambrant::sudoku
{
    auto SolverFactory::create( std::string_view name,
                                const Logger&    logger) const
        -> std::expected<std::unique_ptr<ISolver>, std::string>
    {
        const auto* creator = kRegistry.find( name);

        if( ! creator)
        {
            return std::unexpected{ "Unknown solver: " + std::string{ name}};
        }

        return (*creator)( logger);
    }

    auto SolverFactory::solverNames() const -> std::vector<std::string>
    {
        return kRegistry.keys();
    }
}