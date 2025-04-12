//
//  Board.hpp
//  Sudoko
//
//  Created by Thomas Rambrant on 2025-04-07.
//
#pragma once

#include <iostream>
#include <array>

#include "SudokuTraits.hpp"

class ISudokuSolver;

//
// The board class
//
class SudokuBoard
{
    public:
    
        using Traits = SudokuTraits;
        
        SudokuBoard( Traits::Board anInitMatrix);
        
        void solve( ISudokuSolver& solver);
            
        friend std::ostream& operator<<(std::ostream& os, const SudokuBoard& board);
    
    private:
    
        Traits::Board mBoard;
};
