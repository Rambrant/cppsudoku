//
//  SudokoBoard.hpp
//
//  Created by Thomas Rambrant on 2025-04-07.
//
#pragma once

#include <iostream>


#include "SudokuTraits.hpp"

class ISudokuReader;
class ISudokuSolver;

//
// The board class
//
class SudokuBoard
{
    public:
    
        using Traits = SudokuTraits;
        
        SudokuBoard( const ISudokuReader& reader);
        
        bool solve( const ISudokuSolver& solver);
            
        friend std::ostream& operator<<(std::ostream& os, const SudokuBoard& board);
    
    private:
    
        Traits::Board mBoard;
};
