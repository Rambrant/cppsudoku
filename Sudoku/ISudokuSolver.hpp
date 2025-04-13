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

//
// The ISudokuSolver class
//
class ISudokuSolver
{
    using Traits = SudokuTraits;

    public:
    
       virtual bool solve( Traits::Board& board) const = 0;
       
       virtual ~ISudokuSolver() = default;
};
