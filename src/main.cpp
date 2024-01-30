/*!
 * @file src/main.cpp
 *
 * @brief This file contains the main function of the program, and
 *          is responsible for parsing command line arguments.
 */

#include <iostream>

#include "error.hpp"
#include "lexer/lexer.hpp"

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
    if (SUCCESS != lexer.lex_file())
    {
        // Report error.
        std::cerr << "Got error" << std::endl;
        return 1;
    }

    for (size_t idx = 0; idx < lexer._tokens.size(); ++idx)
    {
        Token t = lexer._tokens.at(idx);
        std::cout << "[" << idx << "]: ";
        std::cout << dbg_tok_names[t._type] << " | " << t._lexeme << " | " << t._linenum << " | " << t._colnum << std::endl;
    }

    std::cout << "Success" << std::endl;
}

/***   end of file   ***/
