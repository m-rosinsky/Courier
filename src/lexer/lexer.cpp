/*!
 * @file src/lexer/lexer.cpp
 *
 * @brief This file contains the implementation for the lexer.
 * 
 *          The lexer's responsibility is to turn raw source into
 *              a sequence of tokens to pass into the parser.
 */

#include <algorithm>
#include <cerrno>
#include <fstream>
#include <map>

#include "lexer.hpp"

/******************************************************************************/

#define DBL_QUOTE       34 // '"'
#define CHAR_COMMENT    35 // '#'

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

/*!
 * @brief This is a helper function to determine binary operator characters.
 */
static bool
is_bin_op (char c)
{
    return ((c == '+') ||
            (c == '-') ||
            (c == '*') ||
            (c == '/') ||
            (c == '='));
}

/*!
 * @brief This is a helper function to determine unary operator characters.
 */
static bool
is_un_op (char c)
{
    return (c == '!');
}

/*!
 * @brief This is a helper function to determine any operator characters.
 */
static bool
is_op (char c)
{
    return is_bin_op(c) || is_un_op(c);
}

/*!
 * @brief This is a helper function to determine multi-comparison operators.
 */
static bool
is_dbl_op (char c)
{
    return ((c == '=') || // ==
            (c == '!'));  // !=
}

/******************************************************************************/

/*!
 * @brief This function performs lexical analysis against a raw file.
 *
 * @return ErrorCode.
 */
void
Lexer::lex_file (void)
{
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
            _err = Error(ERR_FILE_NOT_FOUND, 0, 0, "");
            goto EXIT;
        }
        _err = Error(ERR_FILE_BAD_OPEN, 0, 0, "");
        goto EXIT;
    }

    // Perform lexical analysis line-by-line.
    while (std::getline(infile, line))
    {
        lex_line(line, line_num);
        if (_err.has_error())
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
        return;
}

/******************************************************************************/

/*!
 * @brief This line performs lexical analysis on a single line.
 *
 * @param __line The line from raw source.
 * @param __line_num The current line number.
 */
void
Lexer::lex_line (const std::string& __line, uint32_t __line_num)
{
    // The column number of the current character.
    uint32_t col_num = 1;

    // Loop over input string.
    for (size_t idx = 0; idx < __line.size(); ++idx)
    {
        char c = __line[idx];

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
            return;
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

        // Numeric Literals.
        // Regex: [-0-9]?[0-9.]*[0-9]
        // Numeric literals are integers and floats, which may start with
        // a negative sign.
        if ((isdigit(c)) ||
            (c == '-' && idx + 1 < __line.size() && isdigit(__line.at(idx+1))))
        {
            // Bookmark the starting column of the numeric literal.
            uint32_t start_col = col_num;

            // String to hold the literal.
            std::string num_str = "";

            // Handle optional negative sign.
            if (c == '-')
            {
                num_str += c;
                c = __line[++idx];
                col_num++;
            }

            // Collect the literal.
            while ((isdigit(c)) || c == '.')
            {
                num_str += c;
                c = __line[++idx];
                col_num++;
            }

            // The last character in the literal must not be a decimal point,
            // and there must not be more than one decimal point.
            if ((num_str.back() == '.') ||
                (std::count(num_str.begin(), num_str.end(), '.') > 1))
            {
                _err = Error(ERR_LEX_INVAL_NUM, __line_num, start_col, num_str);
                goto EXIT;
            }

            // Differentiate ints from floats.
            TokenType t = (num_str.find('.') != std::string::npos)
                ? TOKEN_LIT_FLOAT : TOKEN_LIT_INT;

            // Create the token.
            _tokens.emplace_back(t, num_str, __line_num, start_col);

            // The index is currently looking at the next char, but the loop
            // will increment idx again, so move it back.
            idx--;
            continue;
        }

        // String literals.
        // Regex. ^"*$".
        // String literals must occur on the same line between two double quotes.
        if (c == DBL_QUOTE)
        {
            // Bookmark the starting column of the string literal.
            uint32_t start_col = col_num;

            // String to hold the literal.
            std::string str = "";

            // Collect the literal.
            bool str_closed = false;
            col_num++;
            for (++idx; idx < __line.size(); ++idx)
            {
                c = __line[idx];
                if (c == DBL_QUOTE)
                {
                    str_closed = true;
                    break;
                }

                // Escape sequences.
                if (c == '\\' && idx + 1 < __line.size())
                {
                    idx++;
                    col_num++;
                    c = __line[idx];

                    // Check valid escape sequences.
                    if (c == 't')
                    {
                        str += '\t';
                    } else if (c == 'r')
                    {
                        str += '\r';
                    } else if (c == 'n')
                    {
                        str += '\n';
                    } else if (c == DBL_QUOTE)
                    {
                        str += '"';
                    } else if (c == '0')
                    {
                        str += '\0';
                    } else {
                        _err = Error(ERR_LEX_INVAL_ESC, __line_num, col_num, "\\" + c);
                        goto EXIT;
                    }

                    col_num++;
                    continue;
                }

                // Add character to string literal.
                str += c;
                col_num++;
            }

            // If the string was not closed, this is an error.
            if (!str_closed)
            {
                _err = Error(ERR_LEX_UNCLOSED_STR, __line_num, start_col, "");
                goto EXIT;
            }

            // Create the token.
            _tokens.emplace_back(TOKEN_LIT_STR, str, __line_num, start_col);
            col_num++;
            continue;
        }

        // Operators.
        // Regex: See operator strings above.
        // Operators can be multicharacter or single character.
        if (is_op(c))
        {
            // Bookmark the starting column of the operator, since it
            // may be multi-character.
            uint32_t start_col = col_num;

            // String to hold the operator lexeme.
            std::string str = "";

            // Add the operator to the string.
            str += c;

            // Check if the operator can be multi-char.
            if ((is_dbl_op(c)) &&
                (idx + 1 < __line.size()) &&
                (__line[idx + 1] == '='))
            {
                str += '=';
                idx++;
                col_num++;
            }

            _tokens.emplace_back(TOKEN_OP, str, __line_num, start_col);
            col_num++;
            continue;
        }

        // Any tokens at this point are considered unrecognized.
        _err = Error(ERR_LEX_UNEXP_CHAR, __line_num, col_num, std::string(1, c));
        goto EXIT;
    }

    EXIT:
        return;
}

/***   end of file   ***/
