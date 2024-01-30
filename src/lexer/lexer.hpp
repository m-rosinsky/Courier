/*!
 * @file src/lexer/lexer.hpp
 *
 * @brief This file contains the implementation for the lexer.
 * 
 *          The lexer's responsibility is to turn raw source into
 *              a sequence of tokens to pass into the parser.
 */

#ifndef _COURIER_SRC_LEXER_LEXER_HPP
#define _COURIER_SRC_LEXER_LEXER_HPP

#include <string>                   // std::string
#include <vector>                   // std::vector

#include "../error.hpp"

/*!
 * @brief This enum enumerates the various token types.
 */
enum TokenType
{
    // Generic.
    TOKEN_IDENT = 0,    // Identifiers.
    TOKEN_OP,           // Operators.
    TOKEN_KW,           // Keywords.

    // Literals.
    TOKEN_LIT_INT,      // Integer literals.
    TOKEN_LIT_FLOAT,    // Floating point literals.
    TOKEN_LIT_STR,      // String literals.

    // Delimiters.
    TOKEN_LBRACE,       // '{'
    TOKEN_RBRACE,       // '}'
    TOKEN_LPAREN,       // '('
    TOKEN_RPAREN,       // ')'
    TOKEN_SEMICOLON,    // ';'
};

static std::vector<std::string> dbg_tok_names = {
    "TOKEN_IDENT",
    "TOKEN_OP",
    "TOKEN_KW",
    "TOKEN_LIT_INT",
    "TOKEN_LIT_FLOAT",
    "TOKEN_LIT_STR",
    "TOKEN_LBRACE",
    "TOKEN_RBRACE",
    "TOKEN_LPAREN",
    "TOKEN_RPAREN",
    "TOKEN_SEMICOLON",
};

/*!
 * @brief This struct defines a token.
 *
 * @param _type The TokenType for the token.
 * @param _lexeme The token's lexeme, if it applies.
 * @param _linenum The token's line number within raw source.
 * @param _colnum The token's column number within raw source.
 */
struct Token
{
    TokenType _type;
    std::string _lexeme;
    uint32_t _linenum;
    uint32_t _colnum;

    // Ctor.
    Token(TokenType __type,
          const std::string& __lexeme,
          uint32_t __linenum,
          uint32_t __colnum)
        : _type(__type), _lexeme(__lexeme),
        _linenum(__linenum), _colnum(__colnum) {}
};

/*!
 * @brief This class defines a lexer object.
 *
 * @param _fname The filename of the source file to lex.
 * @param _tokens The sequence of tokens generated from lexical analysis.
 */
class Lexer
{
private:
    std::string _fname;

public:
    std::vector<Token> _tokens;

public:
    // Ctors.
    Lexer() = delete;
    Lexer(const std::string& __fname)
        : _fname(__fname) {}

    // Methods.
    ErrorCode lex_file (void);
    ErrorCode lex_line (const std::string& __line, uint32_t __line_num);
};

#endif // _COURIER_SRC_LEXER_LEXER_HPP

/***   end of file   ***/
