//
//  SudokuFileReader.hpp
//
//  Created by Thomas Rambrant on 2025-04-24.
//
#pragma once

#include "ISudokuReader.hpp"
    
/**
 * @brief A reader that reads a board from an input stream. The reads all digits, '.' is zero' even hexadecimal if the board is more than 9x9.
 */
class SudokuAsciiReader final : public ISudokuReader
{
    public:
        
        /**
         * @brief Constructs the class with for an in stream.
         * @param is The existing in stream.
         */
        explicit SudokuAsciiReader( std::istream& is);
        
        /**
         * @brief Reads the Sudoku board
         * @return The @ref SudokuTraits.Board. Usually a 9x9 grid with values 1-9 (zero representing no value).
         */
        [[nodiscard]]
        auto read() const -> Traits::Board override;

    private:
    
        std::istream& mStream;
};