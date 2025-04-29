//
//  ISudokuSolver.hpp
//
//  Created by Thomas Rambrant on 2025-04-07.
//
#pragma once

#include "SudokuTraits.hpp"

//------------------------------------------------------------------------------
//
class ISudokuSolver
{
    using Traits = SudokuTraits;

    public:
    
       virtual auto solve( Traits::Board& board) const -> Traits::BoardResult = 0;
       
       virtual ~ISudokuSolver() = default;
};
