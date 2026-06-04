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
     * @brief A writer interface for classes that can write a Sudoku board to some sort of destination.
     */
    class IWriter
    {
        public:

            /// @brief Alias to shorten access to @ref SudokuTraits.
            using Traits = SudokuTraits;

            /**
             * @brief Base class constructor that initiates the logger member to be used by the subclasses
             * @param logger The logger instance. Must be valid during the lifetime of the writers
             */
            explicit IWriter( const Logger& logger) : mLogger( logger) {};
            virtual ~IWriter() = default;

            /**
             * @brief A method that writes a Sudoku board to destination.
             * @param board The board to print ou to the stream
             */
            virtual auto write( const Traits::Board& board) const -> void = 0;


        protected:

            /// @brief internal logger reference to be used by the subclasses to write log messages to
            const Logger& mLogger;
    };

    //
    //  WriterPlugin concept
    //

    /**
     * @brief Compile-time contract for every @ref IWriter implementation that
     *        participates in @ref WriterFactory auto-registration.
     *
     * Mirrors @ref ReaderPlugin exactly — see its documentation for rationale
     * and the P2996 migration note.
     *
     * @par Example
     * @code
     * class BlockWriter final : public IWriter {
     * public:
     *     static constexpr std::string_view formatName = "block";
     *     BlockWriter( std::ostream&, const Logger&);
     *     auto write( const Traits::Board&) const -> void override;
     * };
     * static_assert( WriterPlugin<BlockWriter>);
     * @endcode
     */
    template<typename T>
    concept WriterPlugin = PluginType<T, IWriter>;
}