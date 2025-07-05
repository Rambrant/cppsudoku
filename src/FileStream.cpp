//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
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

        if( mode == Mode::ReadWrite && ! std::filesystem::exists( fileName))
        {
            std::ofstream tmp( fileName); // This fixes a C++ quirk that ios::in combined with ios::out doesn't create the file
        }

        open( fileName, openMode);

        if( ! is_open( ))
        {
            throw std::runtime_error( "Failed to open file: " + fileName);
        }
    }
}