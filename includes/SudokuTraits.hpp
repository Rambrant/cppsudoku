//
//  SudokuTraits.hpp
//
//  Created by Thomas Rambrant on 2025-04-11.
//
#pragma once

#include <array>
#include <tuple>

#include "SudokuUtil.hpp"


//------------------------------------------------------------------------------
//
struct SudokuTraits
{
        static constexpr int  BOARD_SIZE      = 9;
        static constexpr int  SUBSECTION_SIZE = 3;
        static constexpr int  NO_VALUE        = 0;
        static constexpr auto INDEX_RANGE     = RangeArray<BOARD_SIZE>(0, 1);
        static constexpr auto VALUE_RANGE     = RangeArray<BOARD_SIZE>(1, 1);

        using BoardResult = std::tuple< bool, std::size_t>;
        using BoardArray  = std::array< int, BOARD_SIZE>;
        using Board       = std::array< BoardArray, BOARD_SIZE>;
};
