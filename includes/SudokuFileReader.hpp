//
//  SudokuFileReader.hpp
//
//  Created by Thomas Rambrant on 2025-04-24.
//
#pragma once

#include "ISudokuReader.hpp"
#include "SudokuTraits.hpp"
    
//------------------------------------------------------------------------------
//
class SudokuFileReader : public ISudokuReader
{
    public:
        
        using Traits = SudokuTraits;
        
        explicit SudokuFileReader( const std::string& fileName);
        
        Traits::Board read() const override;

    private:
    
        mutable Traits::Board mInitBoard;
};