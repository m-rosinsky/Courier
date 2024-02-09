/*!
 * @file src/parser/parser.hpp
 *
 * @brief This file contains the implementation for the language parser.
 * 
 *          The parser's responsibilities break down into two jobs:
 * 
 *              1. Report errors about incorrectly formatted programs.
 *              2. Form a parse tree from correctly formatted programs.
 * 
 *          The parser is a top-down LL(1) parser.
 */

#ifndef _COURIER_SRC_PARSER_PARSER_HPP
#define _COURIER_SRC_PARSER_PARSER_HPP

#include <map>
#include <memory>
#include <stack>
#include <vector>

#include "../lexer/lexer.hpp"
#include "../error.hpp"

struct ParseNode
{
    Token _tok;
    std::unique_ptr<ParseNode> _left;
    std::unique_ptr<ParseNode> _right;

    ParseNode(Token __tok)
        : _tok(__tok) {}
};

enum Symbol
{
    // Non-terminals.
    NT_PROGRAM = 0,
    NT_EXPR_LIST,
    NT_EXPR,
    NT_TYPE,

    // Terminals.
    T_TYPE,
    T_IDENT,
    T_ASSIGN,       // '='
    T_SEMICOLON,    // ';'

    // End of input.
    T_END,

    // Invalid.
    T_INVALID
};

class Parser
{
private:
    std::map<Symbol, std::map<Symbol, Symbol>> _table;
    std::stack<Symbol> _stack;

public:
    Error _err;
    std::unique_ptr<ParseNode> _root;

public:
    Parser();

    void parse (const std::vector<Token>& __tokens);
};

#endif // _COURIER_SRC_PARSER_PARSER_HPP

/***   end of file   ***/
