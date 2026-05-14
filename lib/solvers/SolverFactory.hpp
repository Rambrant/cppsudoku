//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <expected>
#include <flat_map>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "ISolver.hpp"

namespace com::rambrant::sudoku
{
    class Logger;

    /**
     * @brief Factory that creates @ref ISolver instances by solver name.
     *
     * @par Registration mechanism (current: compile-time type list)
     * The registry is populated in the constructor by folding over
     * @ref SolverList, a @c std::tuple of all @ref SolverPlugin types.
     * Each type contributes its @c solverName key and a constructor wrapper.
     *
     * @par Adding a new solver
     * Edit @ref SolverList.hpp only — include the new header and append the
     * type to @c SolverList.  Nothing else needs to change.
     *
     * @par P2996 migration path
     * When Clang supports C++26 static reflection, @ref SolverList.hpp is
     * deleted entirely.  The constructor discovers all @ref SolverPlugin types
     * in the @c com::rambrant::sudoku namespace automatically.  The public
     * interface of this class is unchanged.  See @ref SolverFactory.cpp for
     * the exact diff.
     *
     * @note Unlike @ref ReaderFactory and @ref WriterFactory, solvers take
     *       only a @ref Logger (no stream), because the board is passed
     *       directly to @ref ISolver::solve.
     */
    class SolverFactory
    {
    public:

        using CreatorFn = std::function<std::unique_ptr<ISolver>( const Logger&)>;

        SolverFactory( const SolverFactory&)            = delete;
        SolverFactory& operator=( const SolverFactory&) = delete;

        /**
         * @brief Returns the Meyer's-singleton instance.
         *
         * The registry is fully populated on first call; every subsequent call
         * is a plain @c static local access.
         */
        static auto instance() -> SolverFactory&;

        /**
         * @brief Constructs the @ref ISolver for the given solver name.
         *
         * @param name    Key as supplied on the command line (e.g. @c "backtracking").
         * @param logger  Logger forwarded verbatim to the concrete solver.
         * @return The solver on success, or an error message if @p name is unknown.
         */
        [[nodiscard]]
        auto create( std::string_view name, const Logger&    logger) const
            -> std::expected<std::unique_ptr<ISolver>, std::string>;

        /**
         * @brief Sorted list of every registered solver name.
         *
         * Feed this to @ref ValuesIn to keep command-line validation in sync
         * with @ref SolverList automatically:
         * @code
         *   solversOpt.setValidator(
         *       ValuesIn( SolverFactory::instance().solverNames()));
         * @endcode
         */
        [[nodiscard]]
        auto solverNames() const -> std::vector<std::string>;

    private:

        SolverFactory();

        std::flat_map<std::string, CreatorFn> mRegistry;
    };

}