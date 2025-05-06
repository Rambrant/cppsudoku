//
//  BackTrackingSolver.hpp
//
//  Created by Thomas Rambrant on 2025-04-12.
//
#pragma once

#include <iostream>

#include "ISudokuSolver.hpp"

/**
 * @brief A Sudoku solver that uses a simple brute force backtracking algorithm
 */
class BackTrackingSolver : public ISudokuSolver
{
    public:
    
        /**
         * @brief Solves the given Sudoku board using a brute force backtracking algorithm.
         * @param board A @ref SudokuTraits.Board.
         * @return A @ref SudokuTraits.BoardResult.
         */
        [[nodiscard]]
        auto solve( Traits::Board& board) const -> Traits::BoardResult override;
    
    private:

        mutable std::size_t mRecursions{ 0};

        bool solver( Traits::Board& board) const;
        bool isValid( Traits::Board& board, int value, int rowPos, int columnPos) const;
        
        static bool rowConstraint( const Traits::Board& board, int value, int rowPos) ;
        static bool columnConstraint( const Traits::Board& board, int value, int columnPos) ;
        static bool sectionConstraint( const Traits::Board& board, int value, int rowPos, int columnPos) ;
        
        static bool check( int value, const Traits::BoardArray& arr) ;
};
