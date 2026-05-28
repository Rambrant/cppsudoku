//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "SolverFactory.hpp"

namespace com::rambrant::sudoku
{
    auto SolverFactory::instance() -> SolverFactory&
    {
        static SolverFactory factory;

        return factory;
    }

    auto SolverFactory::solverNames() const -> std::vector<std::string>
    {
        return mRegistry.keys();
    }
}