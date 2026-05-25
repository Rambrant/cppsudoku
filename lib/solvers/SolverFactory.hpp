//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <expected>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "ISolver.hpp"
#include "factorybase//PluginRegistry.hpp"

namespace com::rambrant::sudoku
{
    class Logger;

    /**
     * @brief Factory that creates @ref ISolver instances by solver name.
     *
     * Inherits singleton access, registry storage, and fold-based registration
     * from @ref PluginRegistry.
     *
     * @note Unlike @ref ReaderFactory and @ref WriterFactory, the @c CreatorFn
     *       takes only a @ref Logger — no stream — because the board is passed
     *       directly to @ref ISolver::solve.
     *
     * @par Registration mechanism (current: compile-time type list)
     * Edit @ref SolverList.hpp only to add a new solver.
     *
     * @par P2996 migration path
     * When Clang supports C++26 static reflection, @ref SolverList.hpp is
     * deleted.  The constructor discovers all @ref SolverPlugin types in the
     * namespace automatically.  The public interface is unchanged.
     */
    class SolverFactory : public PluginRegistry<SolverFactory,
                              std::function<std::unique_ptr<ISolver>( const Logger&)>>
    {
    public:
        using CreatorFn = std::function<std::unique_ptr<ISolver>( const Logger&)>;

        /**
         * @brief Constructs the @ref ISolver for the given solver name.
         *
         * @param name    Key as supplied on the command line (e.g. @c "backtracking").
         * @param logger  Logger forwarded verbatim to the concrete solver.
         * @return The solver on success, or an error message if @p name is unknown.
         */
        [[nodiscard]]
        auto create( std::string_view name,
                     const Logger&    logger) const
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
        friend class PluginRegistry<SolverFactory, CreatorFn>;

        template<SolverPlugin T>
        static auto makeCreator() -> CreatorFn;
    };
}