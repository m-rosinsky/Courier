/*!
 * @file src/error.hpp
 *
 * @brief This file is responsible for error handling in courier.
 */

#ifndef _COURIER_SRC_ERROR_HPP
#define _COURIER_SRC_ERROR_HPP

#include <string>
#include <vector>

/*!
 * @brief This enum enumerates the various error types that can be thrown.
 */
enum ErrorCode
{
    // Non-error.
    SUCCESS = 0,

    // Raw file errors.
    ERR_FILE_NOT_FOUND,
    ERR_FILE_BAD_OPEN,

    // Lexical errors.
    ERR_LEX_UNEXP_CHAR,     // Unexpected character.
    ERR_LEX_INVAL_NUM,      // Invalid numeric literal, such as 1.23.4.
    ERR_LEX_INVAL_ESC,      // Invalid escape sequence in string literal.
    ERR_LEX_UNCLOSED_STR,   // Unclosed string literal.
};

/*!
 * @brief This class contains details of a reported error.
 *
 * @param _code The ErrorCode associated with the error.
 * @param _line_num The line number the error occurred on. 0 if N/A.
 * @param _col_num The column number the error occurred on. 0 if N/A.
 * @param _msg An optional error message to be displayed.
 */
class Error
{
private:
    ErrorCode _code;
    uint32_t _line_num;
    uint32_t _col_num;
    std::string _msg;

public:
    Error()
        : _code(SUCCESS), _line_num(0), _col_num(0), _msg("") {}
    Error(ErrorCode __code,
          uint32_t __line_num, uint32_t __col_num,
          const std::string& __msg)
        : _code(__code), _line_num(__line_num), _col_num(__col_num), _msg(__msg) {}

    inline bool
    has_error (void) const noexcept { return _code != SUCCESS; }

    void
    report (void) const noexcept;
};

#endif // _COURIER_SRC_ERROR_HPP

/***   end of file ***/
