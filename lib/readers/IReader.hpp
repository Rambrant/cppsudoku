//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once


#include "core/SudokuTraits.hpp"
#include "core/PluginRegistry.hpp"

namespace com::rambrant::sudoku
{
    class Logger;

    /**
     * @brief Abstract base for all Sudoku board readers.
     *
     * Concrete implementations must also satisfy @ref ReaderPlugin to be
     * eligible for registration with @ref ReaderFactory.
     */
    class IReader
    {
        public:

            /// @brief Alias to shorten access to @ref SudokuTraits.
            using Traits = SudokuTraits;

            /**
             * @brief Base class constructor that initiates the logger member to be used by the subclasses
             * @param logger The logger instance. Must be valid during the lifetime of the readers
             */
            explicit IReader( const Logger& logger) : mLogger( logger) {};
            virtual  ~IReader() = default;

            /**
             * @brief A method that reads a Sudoku board from a source.
             * @return An initialized @ref SudokuTraits::Board. Usually a 9x9 grid with values 1-9 (zero representing no value).
             */
            [[nodiscard]]
            virtual auto read() const -> Traits::Board = 0;

        protected:

            /// @brief internal logger reference to be used by the subclasses to write log messages to
            const Logger& mLogger;
    };
    //
    //  ReaderPlugin concept
    //

    /**
     * @brief Compile-time contract for every @ref IReader implementation that
     *        participates in @ref ReaderFactory auto-registration.
     *
     * A type satisfies @c ReaderPlugin when it:
     *   - publicly derives from @ref IReader, and
     *   - exposes @c static constexpr std::string_view formatName.
     *
     * The name is the exact key users pass on the command line (e.g. @c "text",
     * @c "json").  @ref ReaderList.hpp verifies every listed type against this
     * concept at build time, so a missing or mis-typed @c formatName is a
     * compile error, not a runtime surprise.
     *
     * @par Example
     * @code
     * class AsciiReader final : public IReader {
     * public:
     *     static constexpr std::string_view  entityName = "text";
     *     AsciiReader( std::istream&, const Logger&);
     *     auto read() const -> Traits::Board override;
     * };
     * static_assert( ReaderPlugin<AsciiReader>);
     * @endcode
     *
     * @par P2996 migration note
     * When C++26 static reflection is available in Clang, @ref ReaderList.hpp
     * will be deleted and @ref ReaderFactory will discover all types in the
     * @c com::rambrant::sudoku namespace that satisfy this concept at compile
     * time — with no hand-maintained list.  The concept itself stays unchanged.
     */
    template<typename T>
    concept ReaderPlugin = PluginType<T, IReader>;
}
