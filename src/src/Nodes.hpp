/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <memory>
#include <string>
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
        BinOpNode(NumberNode left_node, Token op_tok, NumberNode right_node);
        ~BinOpNode() {};

        std::string repr();

    private:
        NumberNode left_node;
        Token op_tok;
        NumberNode right_node;
};
