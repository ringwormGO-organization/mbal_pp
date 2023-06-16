/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "Error.hpp"
#include "Position.hpp"
#include "Token.hpp"

template<typename C, typename T>
bool contains(C&& c, T e);

class Lexer
{
    public:
        Lexer(std::string fn, std::string text);
        ~Lexer();

        void advance();
        std::tuple<std::vector<Token>, std::shared_ptr<Error>> make_tokens();

        Token make_number();
        Token make_identifier();
        
        std::tuple<Token, std::shared_ptr<Error>> make_not_equals();
        Token make_equals();
        Token make_less_than();
        Token make_greater_than();

    private:
        std::string fn = "";
        std::string text = "";

        char current_char = '\0';
        std::shared_ptr<Position> pos = std::make_shared<Position>(0, 0, 0, "", "");

        std::string DIGITS = "0123456789";

        bool first_run = true;
};
