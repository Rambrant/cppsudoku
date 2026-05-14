//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

//
//   HOW TO ADD A NEW WRITER FORMAT
//
//   1. Create MyWriter.hpp / MyWriter.cpp.
//      The class must:  (a) inherit from IWriter
//                       (b) declare static constexpr std::string_view
//                               formatName = "myformat";
//
//   2. #include "MyWriter.hpp" below.
//   3. Add MyWriter to WriterList.
//
//   P2996 MIGRATION — delete this entire file when Clang supports P2996.
//   See WriterFactory.cpp for the full constructor migration diff.
//

#include <tuple>

#include "BlockWriter.hpp"
#include "JsonWriter.hpp"
#include "LineWriter.hpp"
#include "PrettyWriter.hpp"

namespace com::rambrant::sudoku
{
    /**
     * @brief Compile-time manifest of every @ref IWriter implementation.
     *
     * Mirrors @ref ReaderList exactly — see its documentation for rationale
     * and the P2996 migration note.
     */
    using WriterList = std::tuple<
        BlockWriter,
        JsonWriter,
        LineWriter,
        PrettyWriter
    >;

    namespace detail
    {
        template<typename... Ts>
        consteval bool allSatisfyWriterPlugin( std::type_identity<std::tuple<Ts...>>)
        {
            return ( WriterPlugin<Ts> && ...);
        }
    }

    static_assert(
        detail::allSatisfyWriterPlugin( std::type_identity<WriterList>{}),
        "Every type in WriterList must satisfy WriterPlugin "
        "(inherit IWriter and expose static constexpr std::string_view formatName)"
    );

}