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
class SudokuFileReader : public ISudokuReader
{
    public:
        
        /**
         * @brief Constructs the class and reads the board.
         * @param fileName The name of an existing file containing a Sudoku board.
         */
        explicit SudokuFileReader( const std::string& fileName);
        
        /**
         * @brief Reads the Sudoku board
         * @return The @ref SudokuTraits.Board. Usually a 9x9 grid with values 1-9 (zero representing no value).
         */
        [[nodiscard]]
        Traits::Board read() const override;

    private:
    
        mutable Traits::Board mInitBoard;
};