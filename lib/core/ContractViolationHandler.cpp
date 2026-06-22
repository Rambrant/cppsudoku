//
// Created by Thomas Rambrant on 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include <contracts>
#include <iostream>

/**
 * @brief Global contract violation handler required by the C++26 contracts facility.
 *
 * GCC calls this function whenever a `pre`, `post`, or `contract_assert` condition
 * evaluates to @c false. The signature is mandated by the standard — it must be
 * defined exactly once in the program.
 *
 * Logs the source location and failing condition to @c std::clog, then calls
 * @c std::abort(). Contract violations represent programming errors, not
 * recoverable runtime conditions, so stack unwinding would be misleading.
 *
 * @param violation  Describes the violated contract: source location, kind
 *                   (pre/post/assert), and the condition text.
 */
void handle_contract_violation( const std::contracts::contract_violation& violation)
{
    std::clog << "Contract violation: "
              << violation.location().file_name() << ":"
              << violation.location().line()       << " — "
              << violation.comment()               << "\n";

    std::abort();
}