//
//  SudokuFileReader.hpp
//
//  Created by Thomas Rambrant on 2025-04-24.
//
#pragma once

#include "ISudokuReader.hpp"
    
/**
 * @brief A board reader that reads a Sudoku board from a file.
 */
class SudokuAsciiReader final : public ISudokuReader
{
    public:
        
        /**
         * @brief Constructs the class and reads the board.
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