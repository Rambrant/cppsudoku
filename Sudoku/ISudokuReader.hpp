//
//  ISudokuReader.hpp
//  Sudoku
//
//  Created by Thomas Rambrant on 2025-04-18.
//
#pragma once

#include "SudokuTraits.hpp"

//
// The ISudokuReader interface
//
class ISudokuReader
{
    using Traits = SudokuTraits;

    public:
    
       virtual Traits::Board read() const = 0;
       
       virtual ~ISudokuReader() = default;
};
