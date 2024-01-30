/*!
 * @file src/error.hpp
 *
 * @brief This file is responsible for error handling in courier.
 */

#ifndef _COURIER_SRC_ERROR_HPP
#define _COURIER_SRC_ERROR_HPP

/*!
 * @brief This enum enumerates the various error types that can be thrown.
 */
enum ErrorCode {
    // Non-error.
    SUCCESS = 0,

    // Raw file errors.
    ERR_FILE_NOT_FOUND,
    ERR_FILE_BAD_OPEN,
};

#endif // _COURIER_SRC_ERROR_HPP

/***   end of file ***/
