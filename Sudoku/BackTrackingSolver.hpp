//
//  BackTrackingSolver.hpp
//
//  Created by Thomas Rambrant on 2025-04-12.
//
#pragma once

#include <iostream>
#include <array>

#include "ISudokuSolver.hpp"
#include "SudokuTraits.hpp"


//
// The BackTrackingSolver class
//
class BackTrackingSolver : public ISudokuSolver
{
    public:
    
        using Traits = SudokuTraits;
        
        bool solve( Traits::Board& board) const override;
    
    private:
    
        bool isValid( Traits::Board& board, int value, int rowPos, int columnPos) const;
        
        bool rowConstraint( const Traits::Board& board, int value, int rowPos) const;
        bool columnConstraint( const Traits::Board& board, int value, int columnPos) const;
        bool sectionConstraint( const Traits::Board& board, int value, int rowPos, int columnPos) const;
        
        bool check( int value, const Traits::BoardArray& arr) const;
};
