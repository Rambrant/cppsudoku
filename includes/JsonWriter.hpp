//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <ostream>

#include "IWriter.hpp"

namespace com::rambrant::sudoku
{
    /**
     * @brief A board writer that prints out a pretty formatted output to the stream
     */
    class JsonWriter final : public IWriter
    {
        public:

            /**
             * @brief Constructs the class that writes the board in JSON format.
             * @param os The existing outstream to write to.
             * @param logger The @ref Logger instance. Must be valid during the lifetime of the writer
             */
            JsonWriter( std::ostream& os, const Logger& logger);

            /**
             * @brief Writes the given board to the stream
             * @param board The board to write out.
             */
            auto write( const Traits::Board& board) const -> void override;

        private:

            std::ostream& mStream;
    };
}
