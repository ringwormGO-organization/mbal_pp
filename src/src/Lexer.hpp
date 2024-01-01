/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "Error.hpp"
#include "Position.hpp"
#include "Token.hpp"

template<typename C, typename T>
bool contains(C&& c, T e);

/**
 * Lexer class
 * @param fn function name
 * @param text user input
*/
class Lexer
{
    public:
        Lexer(std::string fn, std::string text);
        ~Lexer();

        /**
         * Sets current_char variable to next character of user input
        */
        void advance();

        /**
         * Make tokens
         * @return std::tuple<std::vector<Token>, std::shared_ptr<Error>>
        */
        std::tuple<std::vector<Token>, std::shared_ptr<Error>> make_tokens();

        /**
         * Make number
         * @return Token
        */
        Token make_number();

        /**
         * Make string
         * @return Token
        */
        Token make_string();

        /**
         * Make identifier
         * @return Token
        */
        Token make_identifier();

        /**
         * Make `-` operator or `->`
         * @return Token
        */
        Token make_minus_or_arrow();
        
        /**
         * Makes not equal stuff
         * @return std::tuple<Token, std::shared_ptr<Error>>
        */
        std::tuple<Token, std::shared_ptr<Error>> make_not_equals();

        /**
         * Makes equals stuff
         * @return Token
        */
        Token make_equals();

        /**
         * Makes less than stuff
         * @return Token
        */
        Token make_less_than();

        /**
         * Makes greater than stuff
         * @return Token
        */
        Token make_greater_than();

    private:
        std::string fn = "";
        std::string text = "";

        char current_char = '\0';                                                           /* current character */
        std::shared_ptr<Position> pos = std::make_shared<Position>(0, 0, 0, "", "");        /* position */

        std::string DIGITS = "0123456789";                                                  /* usable base-10 digits */

        bool first_run = true;                                                              /* first time in this class? */
};
