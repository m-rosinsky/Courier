/*!
 * @file src/main.cpp
 *
 * @brief This file contains the main function of the program, and
 *          is responsible for parsing command line arguments.
 */

#include <iostream>

#include "error.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"

/*!
 * @brief This function shows program usage.
 */
void
show_usage (void)
{
    std::cout << "Usage: courier filename" << std::endl;
}

int main(int argc, char * argv[])
{
    // Validate the argument number.
    if (2 != argc)
    {
        show_usage();
        return 1;
    }

    // Create the Lexer object.
    Lexer lexer(argv[1]);
    lexer.lex_file();
    if (lexer._err.has_error())
    {
        lexer._err.report();
        return 1;
    }

    for (size_t idx = 0; idx < lexer._tokens.size(); ++idx)
    {
        Token t = lexer._tokens.at(idx);
        std::cout << "[" << idx << "]:\t";
        std::cout << dbg_tok_names[t._type] << "\t| " << t._lexeme << "\t| " << t._linenum << "\t| " << t._colnum << std::endl;
    }

    Parser parser;
    parser.parse(lexer._tokens);
    if (parser._err.has_error())
    {
        parser._err.report();
        return 1;
    }

    std::cout << "Success" << std::endl;
}

/***   end of file   ***/
