//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

//
//  HOW TO ADD A NEW SOLVER
//
//  1. Create MySolver.hpp / MySolver.cpp.
//     The class must:  (a) inherit from ISolver
//                      (b) declare static constexpr std::string_view
//                              solverName = "myname";
//
//  2. #include "MySolver.hpp" below.
//  3. Add MySolver to SolverList.
//
//  Nothing else changes: SolverFactory folds over SolverList at startup,
//  and main() derives the allowed-name list from the factory, so the new
//  solver automatically appears in --help and is accepted on the command
//  line.
//
//
//  P2996 MIGRATION — delete this entire file when Clang supports P2996.
//  See SolverFactory.cpp for the full constructor migration diff.
//
#include <tuple>

#include "BackTrackingSolver.hpp"
#include "ConstraintPropagationSolver.hpp"

namespace com::rambrant::sudoku
{
    /**
     * @brief Compile-time manifest of every @ref ISolver implementation.
     *
     * @ref SolverFactory folds over this tuple in its constructor to populate
     * the runtime registry.  Every type's @c solverName and constructor are
     * resolved statically — the only runtime work is inserting into the map.
     *
     * @note When P2996 reflection lands in Clang, delete this file and update
     *       the factory constructor as documented in @ref SolverFactory.cpp.
     */
    using SolverList = std::tuple<
        BackTrackingSolver,
        ConstraintPropagationSolver
    >;

    namespace detail
    {
        template<typename... Ts>
        consteval bool allSatisfySolverPlugin( std::type_identity<std::tuple<Ts...>>)
        {
            return ( SolverPlugin<Ts> && ...);
        }
    }

    static_assert(
        detail::allSatisfySolverPlugin( std::type_identity<SolverList>{}),
        "Every type in SolverList must satisfy SolverPlugin "
        "(inherit ISolver and expose static constexpr std::string_view solverName)"
    );

}