//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "SolverFactory.hpp"

#include "core/Logger.hpp"
#include "SolverList.hpp"   // ← delete this line when migrating to P2996

namespace com::rambrant::sudoku
{
    template<SolverPlugin T>
    auto SolverFactory::makeCreator() -> SolverFactory::CreatorFn
    {
        return []( const Logger& logger) -> std::unique_ptr<ISolver>
        {
            return std::make_unique<T>( logger);
        };
    }

    SolverFactory::SolverFactory()
    {
        populate(
            std::type_identity<SolverList>{},
            []<typename T>{ return T::solverName; }
        );
    }

    auto SolverFactory::create( std::string_view name,
                                const Logger&    logger) const
        -> std::expected<std::unique_ptr<ISolver>, std::string>
    {
        const auto* creator = findCreator( name);
        if( !creator)
            return std::unexpected{ "Unknown solver: " + std::string{ name}};
        return (*creator)( logger);
    }

    auto SolverFactory::solverNames() const -> std::vector<std::string>
    {
        return registeredKeys();
    }
}