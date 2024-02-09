/*!
 * @file src/parser/parser.cpp
 *
 * @brief The grammar for the language is described below. Non-terminals
 *          appear in all caps. Terminals appear in lowercase.
 * 
 *          PROGRAM : EXPR_LIST
 * 
 *          EXPR_LIST : empty
 *                    | EXPR
 *                    | EXPR EXPR_LIST
 * 
 *          EXPR : TYPE IDENT = EXPR ;
 *               | TYPE IDENT ;
 * 
 *          TYPE : type_uint
 *               | type_int
 */

#include "parser.hpp"

/******************************************************************************/

static std::map<TokenType, Symbol> tok_to_sym = {
    {TOKEN_TYPE_INT, T_TYPE},
    {TOKEN_TYPE_UINT, T_TYPE},
    {TOKEN_OP, T_ASSIGN},
};

/******************************************************************************/

/*!
 * @brief This is a helper function to determine if a symbol is a terminal.
 */
bool
is_terminal (Symbol sym) { return sym >= T_TYPE; }

/******************************************************************************/

/*!
 * @brief This default constructor for the parser.
 *
 *          This function fills the parse table.
 */
Parser::Parser ()
{
    _table[NT_PROGRAM][T_TYPE] = NT_EXPR_LIST;

    _table[NT_EXPR_LIST][T_END] = T_END;    // [EXPR_LIST, $] -> empty
    _table[NT_EXPR_LIST][T_TYPE] = NT_EXPR; // [EXPR_LIST, type] -> EXPR

    _table[NT_EXPR][T_TYPE] = NT_TYPE; // [EXPR, type] -> TYPE

    _table[NT_TYPE][T_TYPE] = T_TYPE;
}

/******************************************************************************/

void
Parser::parse (const std::vector<Token>& __tokens)
{
    // Initialize the stack with the end symbol and the start
    // production.
    _stack.push(T_END);
    _stack.push(NT_PROGRAM);

    // This points to the current token we are assessing.
    auto it = __tokens.begin();

    // This holds the current and next symbols.
    Symbol cur_sym = T_INVALID;
    Symbol next_sym = T_INVALID;

    // Parse.
    while (!_stack.empty())
    {
        // Pop from stack.
        cur_sym = _stack.top();
        _stack.pop();

        printf("Current Symbol: %d\n", cur_sym);

        if (is_terminal(cur_sym))
        {
            // If curreny sym is terminal, compare it with next token.
            if (it == __tokens.end())
            {
                printf("ERROR1\n");
                return;
            }

            // Get the next symbol for the stack.
            next_sym = tok_to_sym[it->_type];

            // Advance the input pointer.
            ++it;
        }
        else
        {
            printf("GOT NON-TERMINAL\n");
            // Current symbol is non-terminal.
            if (it == __tokens.end())
            {
                printf("ERROR2\n");
                return;
            }

            // Get the next symbol for the stack.
            Symbol tok_sym = tok_to_sym[it->_type];
            printf("Token symbol: %d\n", tok_sym);
            next_sym = _table[cur_sym][tok_sym];
        }

        printf("Next Symbol: %d\n", next_sym);

        // If lookup fails, report a syntax error.
        if ((next_sym == T_INVALID) ||
            (next_sym == 0))
        {
            _err = Error(ERR_SYNTAX, it->_linenum, it->_colnum, it->_lexeme);
            return;
        }

        // If end is reached, parsing is successful.
        if (next_sym == T_END)
        {
            printf("SUCCESS\n");
            return;
        }

        // Push the next symbol onto the stack.
        _stack.push(next_sym);
    }
}

/***   end of file   ***/
