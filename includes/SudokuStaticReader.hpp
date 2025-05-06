//
//  SudokuStaticReader.hpp
//
//  Created by Thomas Rambrant on 2025-04-18.
//
#pragma once

#include "ISudokuReader.hpp"
    
/**
 * @brief A Sudoku board reader that "reads" from a statically initialized SudokuTraits.Board instance.
 */
class SudokuStaticReader : public ISudokuReader
{
    public:

        /**
         * @brief Constructs the class and reads the board.
         * @param board An existing @ref SudokuTraits.Board.
         */
        explicit SudokuStaticReader( const Traits::Board& board);

        /**
         * @brief Reads the Sudoku board
         * @return The @ref SudokuTraits.Board. Usually a 9x9 grid with values 1-9 (zero representing no value).
         */
        [[nodiscard]]
        Traits::Board read() const override;

    private:
    
        const Traits::Board& mInitBoard;
};
