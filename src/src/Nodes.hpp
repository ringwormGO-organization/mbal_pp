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

class NumberNode;
class BinOpNode;
class UnaryOpNode;

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
        BinOpNode(std::variant<NumberNode*, BinOpNode*, UnaryOpNode*> left_node, Token op_tok, std::variant<NumberNode*, BinOpNode*, UnaryOpNode*> right_node);
        ~BinOpNode() {};

        std::string repr();

    public:
        std::variant<NumberNode*, BinOpNode*, UnaryOpNode*> left_node;
        Token op_tok;
        std::variant<NumberNode*, BinOpNode*, UnaryOpNode*> right_node;
};

class UnaryOpNode
{
    public:
        UnaryOpNode(Token op_tok, std::variant<NumberNode*, BinOpNode*, UnaryOpNode*> node);
        ~UnaryOpNode();

        std::string repr();

    public:
        Token op_tok;
        std::variant<NumberNode*, BinOpNode*, UnaryOpNode*> node;
};
