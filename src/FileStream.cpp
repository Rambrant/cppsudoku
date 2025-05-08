//
// Created by Thomas Rambrant on 2025-05-08.
//
#include <stdexcept>

#include "FileStream.hpp"

FileStream::FileStream( const std::string& fileName, Mode mode)
{
    std::ios_base::openmode openMode =
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