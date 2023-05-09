/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include "Token.hpp"
#include "Nodes.hpp"

static Token current_tok = Token(TT::NUL);

class Parser
{
    public:
        Parser(std::vector<Token> tokens);
        virtual ~Parser();

        Token advance();
        std::variant<NumberNode, BinOpNode> parse();

        NumberNode factor();
        std::variant<NumberNode, BinOpNode> term();
        std::variant<NumberNode, BinOpNode> expr();

        template <int N>
        std::variant<NumberNode, BinOpNode> bin_op(std::function<std::variant<NumberNode, BinOpNode>()> func, TT ops[N]);

        std::vector<Token> tokens;
    private:
        size_t tok_idx = 1;

        template <int N>
        bool contains(TT current, TT ops[N]);
};
