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
// The board class
//
class SudokuBoard
{
    public:
    
        using Traits = SudokuTraits;
        
        SudokuBoard( Traits::Board anInitMatrix);
        
        void solve();
            
        friend std::ostream& operator<<(std::ostream& os, const SudokuBoard& board);
    
    private:
    
        Traits::Board mBoard;
        
        bool solve( Traits::Board& board);
        bool isValid( Traits::Board& board, int value, int rowPos, int columnPos);
        
        bool rowConstraint( const Traits::Board& board, int value, int rowPos);
        bool columnConstraint( const Traits::Board& board, int value, int columnPos);
        bool sectionConstraint( const Traits::Board& board, int value, int rowPos, int columnPos);
        
        bool check( int value, const std::array<int, Traits::BOARD_SIZE>& arr);
};
