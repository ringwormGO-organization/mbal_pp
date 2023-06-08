/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "Error.hpp"
#include "Position.hpp"
#include "Token.hpp"

class Lexer
{
    public:
        Lexer(std::string fn, std::string text);
        ~Lexer();

        void advance();
        std::tuple<std::vector<Token>, std::shared_ptr<Error>> make_tokens();
        Token make_number();

    private:
        std::string fn = "";
        std::string text = "";

        char current_char = '\0';
        Position pos;

        std::string DIGITS = "0123456789";
};
