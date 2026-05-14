//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#pragma once

//
//   HOW TO ADD A NEW READER FORMAT
//
//   1. Create MyReader.hpp / MyReader.cpp.
//      The class must:  (a) inherit from IReader
//                       (b) declare static constexpr std::string_view
//                               formatName = "myformat";
//
//   2. #include "MyReader.hpp" below.
//   3. Add MyReader to ReaderList.
//
//   Nothing else changes: ReaderFactory folds over ReaderList at startup,
//   and main() derives the allowed-format list from the factory, so the
//   new format automatically appears in --help and is accepted on the
//   command line.
//
//   P2996 MIGRATION — delete this entire file when Clang supports P2996
//
//   ReaderFactory.cpp will discover plugins automatically via reflection:
//
//     consteval auto discoverReaderPlugins()
//     {
//         std::vector<std::meta::info> found;
//         for ( auto r : std::meta::members_of( ^com::rambrant::sudoku))
//             if ( std::meta::is_type( r) && ReaderPlugin<[:r:]>)
//                 found.push_back( r);
//         return found;
//     }
//
//   See ReaderFactory.cpp for the full constructor migration diff.
//

#include <tuple>

#include "AsciiReader.hpp"
#include "JsonReader.hpp"

namespace com::rambrant::sudoku
{
    /**
     * @brief Compile-time manifest of every @ref IReader implementation.
     *
     * @ref ReaderFactory folds over this tuple in its constructor to populate
     * the runtime registry.  Every type's @c formatName and constructor are
     * resolved statically — the only runtime work is inserting into the map.
     *
     * @note This type alias is the sole item that @ref ReaderFactory.cpp
     *       depends on from this file.  When P2996 reflection lands in Clang,
     *       delete this file and update the factory constructor as documented
     *       in @ref ReaderFactory.cpp.
     */
    using ReaderList = std::tuple<
        AsciiReader,
        JsonReader
    >;

    // -------------------------------------------------------------------------
    // Compile-time guard: every type in ReaderList must satisfy ReaderPlugin.
    // If you add a class without a valid formatName, the build fails here with
    // a clear message rather than silently at runtime.
    // -------------------------------------------------------------------------
    namespace detail
    {
        template<typename... Ts>
        consteval bool allSatisfyReaderPlugin( std::type_identity<std::tuple<Ts...>>)
        {
            return ( ReaderPlugin<Ts> && ...);
        }
    }

    static_assert(
        detail::allSatisfyReaderPlugin( std::type_identity<ReaderList>{}),
        "Every type in ReaderList must satisfy ReaderPlugin "
        "(inherit IReader and expose static constexpr std::string_view formatName)"
    );
}