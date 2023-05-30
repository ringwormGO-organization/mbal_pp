/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include "Token.hpp"
    
class NumberNode
{
    public:
        NumberNode(Token tok);
        virtual ~NumberNode() {};

        std::string repr();

    public:
        Token tok;
};

class BinOpNode
{
    public:
        BinOpNode(std::variant<std::monostate, NumberNode*, BinOpNode*> left_node, Token op_tok, std::variant<std::monostate, NumberNode*, BinOpNode*> right_node);
        ~BinOpNode() {};

        std::string repr();

    private:
        std::variant<std::monostate, NumberNode*, BinOpNode*> left_node;
        Token op_tok;
        std::variant<std::monostate, NumberNode*, BinOpNode*> right_node;
};

class UnaryOpNode
{
    public:
        UnaryOpNode(Token op_tok, std::variant<std::monostate, NumberNode*, BinOpNode*> node);
        ~UnaryOpNode();

        std::string repr();

    private:
        Token op_tok;
        std::variant<std::monostate, NumberNode*, BinOpNode*> node;
};
