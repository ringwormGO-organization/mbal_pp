/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
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
class VarAccessNode;
class varAssignNode;
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

        std::shared_ptr<Position> pos_start;
        std::shared_ptr<Position> pos_end;
};

class VarAccessNode
{
    public:
        VarAccessNode(Token var_name_tok);
        ~VarAccessNode();

    public:
        Token var_name_tok;

        std::shared_ptr<Position> pos_start;
        std::shared_ptr<Position> pos_end;
};

class VarAssignNode
{
    public:
        VarAssignNode(Token var_name_tok, std::shared_ptr<NumberNode> value_node);
        ~VarAssignNode();

    public:
        Token var_name_tok;
        std::shared_ptr<NumberNode> value_node;
        
        std::shared_ptr<Position> pos_start;
        std::shared_ptr<Position> pos_end;
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

        std::shared_ptr<Position> pos_start;
        std::shared_ptr<Position> pos_end;
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

        std::shared_ptr<Position> pos_start;
        std::shared_ptr<Position> pos_end;
};
