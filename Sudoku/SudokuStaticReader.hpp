//
//  SudokuStaticReader.hpp
//  Sudoku
//
//  Created by Thomas Rambrant on 2025-04-18.
//
#pragma once

#include "ISudokuReader.hpp"
#include "SudokuTraits.hpp"
    
class SudokuStaticReader : public ISudokuReader
{
    public:
        
        using Traits = SudokuTraits;
        
        SudokuStaticReader( const Traits::Board& board);
        
        Traits::Board read() const override;

    private:
    
        const Traits::Board& mInitBoard;
};
;
