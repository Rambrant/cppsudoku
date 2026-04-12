//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <istream>

#include "IReader.hpp"

namespace com::rambrant::sudoku
{
    /**
     * @brief A reader that reads a board from an input stream. Reads all digits, '.' interpreted as zero even hexadecimals if the board is more than 9x9.
     */
    class AsciiReader final : public IReader
    {
        public:

            /**
             * @brief Constructs the class with for an instream and a Logger
             * @param is The existing instream to read from
             * @param logger The @ref Logger instance. Must be valid during the lifetime of the readers
             */
            AsciiReader( std::istream& is, const Logger& logger);

            /**
             * @brief Reads the Sudoku board
             * @return The @ref SudokuTraits.Board. Usually a 9x9 grid with values 1-9 (zero representing no value).
             */
            [[nodiscard]]
            auto read() const -> Traits::Board override;

        private:

            std::istream& mStream;
    };
}