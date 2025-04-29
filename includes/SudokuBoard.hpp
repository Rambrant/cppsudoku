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

//------------------------------------------------------------------------------
//
class SudokuBoard
{
    public:
    
        using Traits = SudokuTraits;
        
        explicit SudokuBoard( const ISudokuReader& reader);
        
        auto solve( const ISudokuSolver& solver) -> Traits::BoardResult;
            
        friend std::ostream& operator<<(std::ostream& os, const SudokuBoard& board);
    
    private:
    
        Traits::Board mBoard;
};
