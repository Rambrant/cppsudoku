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

//------------------------------------------------------------------------------
//
class BackTrackingSolver : public ISudokuSolver
{
    public:
    
        using Traits = SudokuTraits;

        auto solve( Traits::Board& board) const -> Traits::BoardResult override;
    
    private:

        mutable std::size_t mRecursions{ 0};

        bool solver( Traits::Board& board) const;
        bool isValid( Traits::Board& board, int value, int rowPos, int columnPos) const;
        
        bool rowConstraint( const Traits::Board& board, int value, int rowPos) const;
        bool columnConstraint( const Traits::Board& board, int value, int columnPos) const;
        bool sectionConstraint( const Traits::Board& board, int value, int rowPos, int columnPos) const;
        
        bool check( int value, const Traits::BoardArray& arr) const;
};
