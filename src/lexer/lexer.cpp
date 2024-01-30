/*!
 * @file src/lexer/lexer.cpp
 *
 * @brief This file contains the implementation for the lexer.
 * 
 *          The lexer's responsibility is to turn raw source into
 *              a sequence of tokens to pass into the parser.
 */

#include <cerrno>
#include <fstream>
#include <map>

#include "lexer.hpp"

/******************************************************************************/

#define CHAR_COMMENT 35 // '#'

/******************************************************************************/

/*!
 * @brief This map maps the delimiters characters to their token types.
 */
static std::map<char, TokenType> delim_map = {
    {'{', TOKEN_LBRACE},
    {'}', TOKEN_RBRACE},
    {'(', TOKEN_LPAREN},
    {')', TOKEN_RPAREN},
    {';', TOKEN_SEMICOLON},
};

/******************************************************************************/

/*!
 * @brief This is a helper function to determine whitespace.
 */
static bool
is_whitespace (char c)
{
    return c == ' ' || c == '\t' || c == '\f' || c == '\v' || c == '\r';
}

/*!
 * @brief This is a helper function to determine alphabetic characters.
 */
static bool
is_alpha (char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c == '_');
}

/*!
 * @brief This is a helper function to determine numeric characters.
 */
static bool
is_num (char c)
{
    return (c >= '0' && c <= '9');
}

/*!
 * @brief This is a helper function to determine alphanumeric characters.
 */
static bool
is_alphanum (char c)
{
    return is_alpha(c) || is_num(c);
}

/******************************************************************************/

/*!
 * @brief This function performs lexical analysis against a raw file.
 *
 * @return ErrorCode.
 */
ErrorCode
Lexer::lex_file (void)
{
    ErrorCode status = SUCCESS;

    // These hold the current line information and line number.
    uint32_t line_num = 1;
    std::string line = "";

    // Attempt to open the file.
    std::ifstream infile;
    infile.open(_fname);

    if (!infile.is_open())
    {
        // Check specific error conditions.
        if (ENOENT == errno)
        {
            status = ERR_FILE_NOT_FOUND;
            goto EXIT;
        }
        status = ERR_FILE_BAD_OPEN;
        goto EXIT;
    }

    // Perform lexical analysis line-by-line.
    while (std::getline(infile, line))
    {
        ErrorCode line_err = lex_line(line, line_num);
        if (SUCCESS != line_err)
        {
            goto EXIT;
        }

        // Increment the line number for next pass.
        line_num++;
    }
    
    EXIT:
        // Ensure infile is closed.
        if (infile.is_open())
        {
            infile.close();
        }
        return status;
}

/*!
 * @brief This line performs lexical analysis on a single line.
 *
 * @param __line The line from raw source.
 * @param __line_num The current line number.
 */
ErrorCode
Lexer::lex_line (const std::string& __line, uint32_t __line_num)
{
    // The current status of the function.
    ErrorCode status = SUCCESS;

    // The column number of the current character.
    uint32_t col_num = 1;

    // Loop over input string.
    for (size_t idx = 0; idx < __line.size(); ++idx)
    {
        char c = __line.at(idx);

        // Whitespace.
        // Regex: '[ \t\f\r\v]+'
        if (is_whitespace(c))
        {
            // Ignore whitespace.
            col_num++;
            continue;
        }

        // Comment '#'.
        // Comment characters will automatically comment out the rest of the
        // line, so we can simply return without further analysis.
        if (CHAR_COMMENT == c)
        {
            return SUCCESS;
        }

        // Delimiters.
        // Regex: [\{\}\(\);]
        // Delimiters do not require a lexeme.
        if (delim_map.count(c) > 0)
        {
            // Get the token type using the hash map.
            TokenType type = delim_map[c];

            // Push the token back.
            _tokens.emplace_back(type, "", __line_num, col_num);

            col_num++;
            continue;
        }

        // Identifiers.
        // Regex: [_a-zA-Z][_a-zA-Z0-9]+
        // Identifiers start with an underscore or a letter, and are used
        // to denote variable names and types.
        if (is_alpha(c))
        {
            // Bookmark the starting column number of the identifier.
            uint32_t start_col = col_num;

            // String to hold the identifier.
            std::string ident_str = "";

            // Collect the identifier.
            while(is_alphanum(c))
            {
                ident_str += c;
                c = __line[++idx];
                col_num++;
            }

            // Form the token from the identifier.
            _tokens.emplace_back(TOKEN_IDENT, ident_str, __line_num, start_col);

            // The index is currently looking at the next char, but the loop
            // will increment idx again, so move it back.
            idx--;
            continue;
        }
    }

    return status;
}

/***   end of file   ***/
