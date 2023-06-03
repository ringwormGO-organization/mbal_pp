/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "Token.hpp"

#include "define.hpp"

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

        Position pos_start;
        Position pos_end;
};

class BinOpNode
{
    public:
        BinOpNode(ALL_VARIANT left_node, Token op_tok, ALL_VARIANT right_node);
        ~BinOpNode() {};

        std::string repr();

    public:
        ALL_VARIANT left_node;
        Token op_tok;
        ALL_VARIANT right_node;

        Position pos_start;
        Position pos_end;
};

class UnaryOpNode
{
    public:
        UnaryOpNode(Token op_tok, ALL_VARIANT node);
        ~UnaryOpNode();

        std::string repr();

    public:
        Token op_tok;
        ALL_VARIANT node;

        Position pos_start;
        Position pos_end;
};
