//
//  Created by Thomas Rambrant, 2025
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

#include <fstream>

namespace com::rambrant::sudoku
{
    /**
     * @brief Encapsulates the concept of a file stream that handles failures in opening the file
     */
    class FileStream : public std::fstream
    {
        public:

            /**
             * @brief The mode the file shall open in
             */
            enum class Mode {
                /**
                 * @brief The file shall be opened for reading
                 */
                Read,
                /**
                 * @brief The file should be opened for writing
                 */
                Write,
                /**
                 * @brief The file should be opened for both reading and writing
                 */
                ReadWrite
            };

            /**
             * @brief Construct a file stream and checks that the file could be opened
             * @param fileName The name of the file
             * @param mode The mode the file should open in. Defaults to @ref Mode::Read
             */
            explicit FileStream( const std::string& fileName, const Mode& mode = Mode::Read);
    };
}