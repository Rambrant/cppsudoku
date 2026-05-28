//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <atomic>

#include "core/SudokuTraits.hpp"
#include "factorybase/PluginRegistry.hpp"

namespace com::rambrant::sudoku
{
    class Logger;

    /**
     * @brief An exception used to signal the premature exit of the solver
     */
    struct CancelledException : std::exception
    {
        /**
         * @brief Returns the cancellation reason as a C string
         */
        [[nodiscard]]
        const char* what() const noexcept override { return "Solver prematurely cancelled"; }
        ~CancelledException() noexcept override = default;
    };

    /**
     * @brief A solver interface for classes that can solve a Sudoku board using some sort of algorithm.
     */
    class ISolver
    {
        public:

            /// @brief Alias to shorten access to @ref SudokuTraits.
            using Traits = SudokuTraits;

            /**
              * @brief Base class constructor that initiates the logger member to be used by the subclasses
              * @param logger The logger instance. Must be valid during the lifetime of the readers
              */
            explicit ISolver( const Logger& logger) : mLogger( logger) {};
            virtual ~ISolver() = default;

            /**
             * @brief Solves the given Sudoku board
             * @param board A SudokuTraits.Board. Usually a 9x9 grid with values 1-9 (zero representing no value).
             * @param cancelFlag A reference to an atomic flag used to signal the premature exit of thread
             * @return A SudokuTraits.BoardResult
             */
            [[nodiscard]]
            virtual auto solve( Traits::Board& board, std::atomic<bool>& cancelFlag ) const -> Traits::BoardResult = 0;

            /**
             * @brief Returns the solver's registered name (e.g. @c "backtracking").
             *
             * Mirrors the @c solverName static constexpr through virtual dispatch
             * so @ref SudokuBoard can identify which solver won the race without
             * knowing the concrete type.
             */
            [[nodiscard]]
            virtual auto name() const noexcept -> std::string_view = 0;

        protected:

            /// @brief internal logger reference to be used by the subclasses to write log messages to
            const Logger& mLogger;
    };

    //
    //  SolverPlugin concept
    //

    /**
     * @brief Compile-time contract for every @ref ISolver implementation that
     *        participates in @ref SolverFactory auto-registration.
     *
     * A type satisfies @c SolverPlugin when it:
     *   - publicly derives from @ref ISolver, and
     *   - exposes @c static constexpr std::string_view solverName.
     *
     * The name is the exact key users pass on the command line
     * (e.g. @c "backtracking", @c "constraint").
     * @ref SolverList.hpp verifies every listed type against this concept at
     * build time, so a missing or mis-typed @c solverName is a compile error.
     *
     * @par Example
     * @code
     * class BackTrackingSolver final : public ISolver {
     * public:
     *     static constexpr std::string_view solverName = "backtracking";
     *     explicit BackTrackingSolver( const Logger&);
     *     auto solve( Traits::Board&, std::atomic<bool>&) const
     *         -> Traits::BoardResult override;
     * };
     * static_assert( SolverPlugin<BackTrackingSolver>);
     * @endcode
     *
     * @par P2996 migration note
     * When C++26 static reflection is available in Clang, @ref SolverList.hpp
     * will be deleted and @ref SolverFactory will discover all types in the
     * @c com::rambrant::sudoku namespace that satisfy this concept automatically.
     * The concept itself stays unchanged.
     */
    template<typename T>
    concept SolverPlugin = PluginType<T, ISolver>;

}