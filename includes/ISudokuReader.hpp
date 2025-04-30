//
//  ISudokuReader.hpp
//
//  Created by Thomas Rambrant on 2025-04-18.
//
#pragma once

#include "SudokuTraits.hpp"

//------------------------------------------------------------------------------
//
class ISudokuReader
{
    using Traits = SudokuTraits;

    public:
    
       virtual Traits::Board read() const = 0;
       
       virtual ~ISudokuReader() = default;
};
