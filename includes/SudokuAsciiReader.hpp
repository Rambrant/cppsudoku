//
//  Created by Thomas Rambrant on 2025-04-24.
//
#pragma once

#include <istream>

#include "ISudokuReader.hpp"
    
namespace com::rambrant::sudoku
{
    /**
     * @brief A reader that reads a board from an input stream. The reads all digits, '.' is zero' even hexadecimal if the board is more than 9x9.
     */
    class SudokuAsciiReader final : public ISudokuReader
    {
        public:

            /**
             * @brief Constructs the class with for an instream and a Logger
             * @param is The existing instream to read from
             * @param logger The @ref Logger instance. Must be valid during the lifetime of the readers
             */
            explicit SudokuAsciiReader( std::istream& is, Logger& logger);

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