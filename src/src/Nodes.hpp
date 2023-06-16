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

/**
 * Number node
 * @param tok tok
*/
class NumberNode
{
    public:
        NumberNode(Token tok);
        virtual ~NumberNode() {};

        std::string repr();

    public:
        Token tok;

        std::shared_ptr<Position> pos_start;            /* starting position */
        std::shared_ptr<Position> pos_end;              /* ending position */
};

/**
 * Variable access node
 * @param var_name_tok variable name token
*/
class VarAccessNode
{
    public:
        VarAccessNode(Token var_name_tok);
        ~VarAccessNode();

    public:
        Token var_name_tok;

        std::shared_ptr<Position> pos_start;            /* starting position */
        std::shared_ptr<Position> pos_end;              /* ending position */
};

/**
 * Variable assign node
 * @param var_name_tok variable name token
 * @param value_node number node
*/
class VarAssignNode
{
    public:
        VarAssignNode(Token var_name_tok, std::shared_ptr<NumberNode> value_node);
        ~VarAssignNode();

    public:
        Token var_name_tok;
        std::shared_ptr<NumberNode> value_node;
        
        std::shared_ptr<Position> pos_start;            /* starting position */
        std::shared_ptr<Position> pos_end;              /* ending position */
};

/**
 * Binary operation node
 * @param left_node left node
 * @param op_tok operator tok
 * @param right_node right node
*/
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

        std::shared_ptr<Position> pos_start;            /* starting position */
        std::shared_ptr<Position> pos_end;              /* ending position */
};

/**
 * Unary operation node
 * @param op_tok operator tok
 * @param node node
*/
class UnaryOpNode
{
    public:
        UnaryOpNode(Token op_tok, ALL_VARIANT node);
        ~UnaryOpNode();

        std::string repr();

    public:
        Token op_tok;
        ALL_VARIANT node;

        std::shared_ptr<Position> pos_start;            /* starting position */
        std::shared_ptr<Position> pos_end;              /* ending position */
};
