#include "ReaderFactory.hpp"

namespace com::rambrant::sudoku
{
    auto ReaderFactory::instance() -> ReaderFactory&
    {
        static ReaderFactory factory;

        return factory;
    }
}