//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <expected>
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
     * @par Registration mechanism (compile-time constexpr array)
     * The registry is a @c constexpr @c std::array of {name, function-pointer}
     * pairs, built and sorted at compile time from @ref SolverList.
     * It lives in @c .rodata — no heap allocation, zero startup cost.
     *
     * @par Adding a new solver
     * Edit @ref SolverList.hpp only — include the new header and append the
     * type to @c SolverList. Nothing else needs to change.
     *
     * @par P2996 migration path
     * When Clang supports C++26 static reflection, @ref SolverList.hpp is
     * deleted entirely. The registry is discovered automatically at compile
     * time. The public interface of this class is unchanged.
     * See @ref SolverFactory.cpp for the exact diff.
     */
    class SolverFactory
    {
    public:

        SolverFactory( const SolverFactory&)            = delete;
        SolverFactory& operator=( const SolverFactory&) = delete;

        /**
         * @brief Returns the singleton instance.
         *
         * The factory itself carries no mutable state — the registry is a
         * global constexpr array. The singleton exists only to preserve the
         * existing call-site interface.
         */
        static auto instance() -> const SolverFactory&;

        /**
         * @brief Constructs the @ref ISolver for the given solver name.
         *
         * @param name    Key as supplied on the command line (e.g. @c "backtracking").
         * @param logger  Logger forwarded verbatim to the concrete solver.
         * @return The solver on success, or an error message if @p name is unknown.
         */
        [[nodiscard]]
        auto create( std::string_view name, const Logger& logger) const
            -> std::expected<std::unique_ptr<ISolver>, std::string>;

        /**
         * @brief Sorted list of every registered solver name.
         */
        [[nodiscard]]
        auto solverNames() const -> std::vector<std::string>;

    private:

        SolverFactory() = default;
    };
}