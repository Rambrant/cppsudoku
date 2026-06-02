//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "WriterFactory.hpp"

#include "core/Logger.hpp"

namespace com::rambrant::sudoku
{
    auto WriterFactory::instance() -> WriterFactory&
    {
        static WriterFactory factory;

        return factory;
    }
}