/*!
 * @file src/error.hpp
 *
 * @brief This file is responsible for error handling in courier.
 */

#include "error.hpp"

#include <iostream>

static std::vector<std::string> error_strings = {
    // Non-error.
    "Success",

    // Raw file errors.
    "File not found",
    "Error opening file",

    // Lexical errors.
    "Unexpected character",
    "Invalid numeric literal",
    "Invalid escape sequence",
    "Unclosed string",
};

/******************************************************************************/

/*!
 * @brief This function reports an error to stderr.
 */
void
Error::report (void) const noexcept
{
    std::cerr << "[\033[31mError\033[0m] ";
    std::cerr << "[Line " << _line_num << ", Col " << _col_num << "] ";
    std::cerr << error_strings[_code];
    if (_msg.length() > 0)
    {
        std::cerr << " '" << _msg << "'";
    }
    std::cerr << std::endl;
}

/***   end of file   ***/
