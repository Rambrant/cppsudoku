//
// Created by Thomas Rambrant on 2025-05-08.
//
#include "FileStream.hpp"

#include <stdexcept>

namespace com::rambrant::sudoku
{
    FileStream::FileStream( const std::string& fileName, const Mode& mode)
    {
        const openmode openMode =
                (mode == Mode::Read)      ? std::ios::in :
                (mode == Mode::Write)     ? std::ios::out | std::ios::trunc :
                (mode == Mode::ReadWrite) ? std::ios::in | std::ios::out :
                                            std::ios::in; // Fallback (shouldn't happen)

        open( fileName, openMode);

        if( ! is_open( ))
        {
            throw std::runtime_error( "Failed to open file: " + fileName);
        }
    }
}