/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <any>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "Error.hpp"
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
        ListNode(std::vector<std::any> element_nodes, std::shared_ptr<Position> pos_start, std::shared_ptr<Position> pos_end);
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
        VarAssignNode(Token var_name_tok, std::any value_node);
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
        BinOpNode(std::any left_node, Token op_tok, std::any right_node);
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
        UnaryOpNode(Token op_tok, std::any node);
        ~UnaryOpNode();

        std::string repr();

    public:
        Token op_tok;
        ALL_VARIANT node;

        std::shared_ptr<Position> pos_start;            /* starting position */
        std::shared_ptr<Position> pos_end;              /* ending position */
};

/**
 * Node for IF statement
 * @param cases cases for IF statement
 * @param else_case ELSE statement/case
*/
class IfNode
{
    public:
        IfNode(std::any cases, std::any else_case=std::any());
        ~IfNode();

    public:
        std::pair<std::vector<std::tuple<ALL_VARIANT, ALL_VARIANT, bool>>, std::any> cases;
        std::pair<ALL_VARIANT, bool> else_case;

        bool is_else_case_empty = false;                /* do we have ELSE statement */

        std::shared_ptr<Position> pos_start;            /* starting position */
        std::shared_ptr<Position> pos_end;              /* ending position */
};

/**
 * Node for FOR statement
 * @param var_name_tok variable name token
 * @param start_value_node node holding start value
 * @param end_value_node node holding end value
 * @param step_value_node node holding step value
 * @param body_node node holding body
 * @param should_return_null should class return null
*/
class ForNode
{
    public:
        ForNode(Token var_name_tok, std::any start_value_node, std::any end_value_node, std::any step_value_node, std::any body_node, bool should_return_null);
        ~ForNode();

    public:
        Token var_name_tok;
        ALL_VARIANT start_value_node;
        ALL_VARIANT end_value_node;
        ALL_VARIANT step_value_node;
        ALL_VARIANT body_node;
        bool should_return_null;

        std::shared_ptr<Position> pos_start;            /* starting position */
        std::shared_ptr<Position> pos_end;              /* ending position */
};

/**
 * Node for WHILE statement
 * @param condition_node node holding condition
 * @param body_node node holding body of WHILE statement
 * @param should_return_null should class return null
*/
class WhileNode
{
    public:
        WhileNode(std::any condition_node, std::any body_node, bool should_return_null);
        ~WhileNode();

    public:
        ALL_VARIANT condition_node;
        ALL_VARIANT body_node;
        bool should_return_null;

        std::shared_ptr<Position> pos_start;            /* starting position */
        std::shared_ptr<Position> pos_end;              /* ending position */
};

/**
 * Node for DO statement, very similar to WHILE statement
 * @param body_node node holding body of DO statement
 * @param condition_node node holding condition
 * @param should_return_null should class return null
*/
class DoNode
{
    public:
        DoNode(std::any body_node, std::any condition_node, bool should_return_null);
        ~DoNode();

    public:
        ALL_VARIANT condition_node;
        ALL_VARIANT body_node;
        bool should_return_null;

        std::shared_ptr<Position> pos_start;            /* starting position */
        std::shared_ptr<Position> pos_end;              /* ending position */
};

/**
 * Node holding functions
 * @param var_name_tok variable name of token
 * @param arg_name_toks vector holding arguments as Token class, a.k.a tokens
 * @param body_node node holding body of function; code which is executed, code which returns a value to caller
 * @param should_return_null should class return null
*/
class FuncDefNode
{
    public:
        FuncDefNode(Token var_name_tok, std::vector<Token> arg_name_toks, std::any body_node, bool should_return_null);
        ~FuncDefNode();

    public:
        Token var_name_tok;
        std::vector<Token> arg_name_toks;
        ALL_VARIANT body_node;
        bool should_return_null;

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
        CallNode(std::any node_to_call, std::vector<std::any> arg_nodes);
        ~CallNode();

    public:
        ALL_VARIANT node_to_call;
        std::vector<ALL_VARIANT> arg_nodes;

        std::shared_ptr<Position> pos_start;            /* starting position */
        std::shared_ptr<Position> pos_end;              /* ending position */
};
