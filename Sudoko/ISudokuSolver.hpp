//
//  Board.hpp
//  Sudoko
//
//  Created by Thomas Rambrant on 2025-04-07.
//
#pragma once

#include <iostream>
#include <array>

//
// The board class
//
class ISudokuSolver;

class SudokuBoard
{
    public:
    
        static constexpr int BOARD_SIZE      = 9;
        static constexpr int SUBSECTION_SIZE = 3;
        static constexpr int NO_VALUE        = 0;

        using BoardArray = std::array< int, BOARD_SIZE>;
        using Board      = std::array< BoardArray, BOARD_SIZE>;

        SudokuBoard( Board anInitMatrix);
        
        void solve( const ISudokuSolver& solver);
            
        friend std::ostream& operator<<(std::ostream& os, const SudokuBoard& board);
    
    private:
    
        Board mBoard;
        
        bool solve( Board& board);
        bool isValid( Board& board, int value, int rowPos, int columnPos);
        
        bool rowConstraint( const Board& board, int value, int rowPos);
        bool columnConstraint( const Board& board, int value, int columnPos);
        bool sectionConstraint( const Board& board, int value, int rowPos, int columnPos);
        
        bool check( int value, const std::array<int, BOARD_SIZE>& arr);
};
