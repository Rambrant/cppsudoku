//
//  BackTrackingSolver.hpp
//  Sudoku
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
        
        bool solve( Traits::Board& board);
    
    private:
    
        bool isValid( Traits::Board& board, int value, int rowPos, int columnPos);
        
        bool rowConstraint( const Traits::Board& board, int value, int rowPos);
        bool columnConstraint( const Traits::Board& board, int value, int columnPos);
        bool sectionConstraint( const Traits::Board& board, int value, int rowPos, int columnPos);
        
        bool check( int value, const Traits::BoardArray& arr);
};
