/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "Token.hpp"

#include "define.hpp"

class NumberNode;
class StringNode;
class ListNode;
class VarAccessNode;
class VarAssignNode;
class BinOpNode;
class UnaryOpNode;
class IfNode;
class ForNode;
class WhileNode;
class DoNode;
class FuncDefNode;
class CallNode;

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
 * String node
 * @param tok tok
*/
class StringNode
{
    public:
        StringNode(Token tok);
        virtual ~StringNode() {};

        std::string repr();

    public:
        Token tok;

        std::shared_ptr<Position> pos_start;            /* starting position */
        std::shared_ptr<Position> pos_end;              /* ending position */
};

/**
 * List node
 * @param tok tok
*/
class ListNode
{
    public:
        ListNode(std::vector<ALL_VARIANT> element_nodes, std::shared_ptr<Position> pos_start, std::shared_ptr<Position> pos_end);
        virtual ~ListNode() {};

        std::string repr();

    public:
        std::vector<ALL_VARIANT> element_nodes;

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
 * @param value_node node
*/
class VarAssignNode
{
    public:
        VarAssignNode(Token var_name_tok, ALL_VARIANT value_node);
        ~VarAssignNode();

    public:
        Token var_name_tok;
        ALL_VARIANT value_node;
        
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

/**
 * Node for IF statment
 * @param cases cases for IF statment
 * @param else_case case for ELSE statment
*/
class IfNode
{
    public:
        IfNode(std::vector<std::pair<ALL_VARIANT, ALL_VARIANT>> cases, ALL_VARIANT else_case);
        ~IfNode();

    public:
        std::vector<std::pair<ALL_VARIANT, ALL_VARIANT>> cases;
        ALL_VARIANT else_case;

        std::shared_ptr<Position> pos_start;            /* starting position */
        std::shared_ptr<Position> pos_end;              /* ending position */
};

/**
 * Node for FOR statment
 * @param var_name_tok variable name token
 * @param start_value_node node holding start value
 * @param end_value_node node holding end value
 * @param step_value_node node holding step value
 * @param body_node node holding body
*/
class ForNode
{
    public:
        ForNode(Token var_name_tok, ALL_VARIANT start_value_node, ALL_VARIANT end_value_node, ALL_VARIANT step_value_node, ALL_VARIANT body_node);
        ~ForNode();

    public:
        Token var_name_tok;
        ALL_VARIANT start_value_node;
        ALL_VARIANT end_value_node;
        ALL_VARIANT step_value_node;
        ALL_VARIANT body_node;

        std::shared_ptr<Position> pos_start;            /* starting position */
        std::shared_ptr<Position> pos_end;              /* ending position */
};

/**
 * Node for WHILE statment
 * @param condition_node node holding condition
 * @param body_node node holding body of WHILE statment
*/
class WhileNode
{
    public:
        WhileNode(ALL_VARIANT condition_node, ALL_VARIANT body_node);
        ~WhileNode();

    public:
        ALL_VARIANT condition_node;
        ALL_VARIANT body_node;

        std::shared_ptr<Position> pos_start;            /* starting position */
        std::shared_ptr<Position> pos_end;              /* ending position */
};

/**
 * Node for DO statment, very similar to WHILE statment
 * @param body_node node holding body of DO statment
 * @param condition_node node holding condition
*/
class DoNode
{
    public:
        DoNode(ALL_VARIANT body_node, ALL_VARIANT condition_node);
        ~DoNode();

    public:
        ALL_VARIANT condition_node;
        ALL_VARIANT body_node;

        std::shared_ptr<Position> pos_start;            /* starting position */
        std::shared_ptr<Position> pos_end;              /* ending position */
};

/**
 * Node holding functions
 * @param var_name_tok variable name of token
 * @param arg_name_toks vector holding arguments as Token class, a.k.a tokens
 * @param body_node node holding body of function; code which is executed, code which returns a value to caller
*/
class FuncDefNode
{
    public:
        FuncDefNode(Token var_name_tok, std::vector<Token> arg_name_toks, ALL_VARIANT body_node);
        ~FuncDefNode();

    public:
        Token var_name_tok;
        std::vector<Token> arg_name_toks;
        ALL_VARIANT body_node;

        std::shared_ptr<Position> pos_start;            /* starting position */
        std::shared_ptr<Position> pos_end;              /* ending position */
};

/**
 * Node handling function call
 * @param node_to_call node which is intended for calling
 * @param arg_nodes nodes' arguments
*/
class CallNode
{
    public:
        CallNode(ALL_VARIANT node_to_call, std::vector<ALL_VARIANT> arg_nodes);
        ~CallNode();

    public:
        ALL_VARIANT node_to_call;
        std::vector<ALL_VARIANT> arg_nodes;

        std::shared_ptr<Position> pos_start;            /* starting position */
        std::shared_ptr<Position> pos_end;              /* ending position */
};
