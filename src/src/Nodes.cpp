/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Nodes.hpp"

#define ANY_CAST(sender, receiver) \
    if (sender.type() == typeid(std::shared_ptr<NumberNode>)) \
    { \
        receiver = std::any_cast<std::shared_ptr<NumberNode>>(sender); \
    } \
    \
    else if (sender.type() == typeid(std::shared_ptr<StringNode>)) \
    { \
        receiver = std::any_cast<std::shared_ptr<StringNode>>(sender); \
    } \
    \
    else if (sender.type() == typeid(std::shared_ptr<ListNode>)) \
    { \
        receiver = std::any_cast<std::shared_ptr<ListNode>>(sender); \
    } \
    \
    else if (sender.type() == typeid(std::shared_ptr<VarAccessNode>)) \
    { \
        receiver = std::any_cast<std::shared_ptr<VarAccessNode>>(sender); \
    } \
    \
    else if (sender.type() == typeid(std::shared_ptr<VarAssignNode>)) \
    { \
        receiver = std::any_cast<std::shared_ptr<VarAssignNode>>(sender); \
    } \
    \
    else if (sender.type() == typeid(std::shared_ptr<BinOpNode>)) \
    { \
        receiver = std::any_cast<std::shared_ptr<BinOpNode>>(sender); \
    } \
    \
    else if (sender.type() == typeid(std::shared_ptr<UnaryOpNode>)) \
    { \
        receiver = std::any_cast<std::shared_ptr<UnaryOpNode>>(sender); \
    } \
    \
    else if (sender.type() == typeid(std::shared_ptr<IfNode>)) \
    { \
        receiver = std::any_cast<std::shared_ptr<IfNode>>(sender); \
    } \
    \
    else if (sender.type() == typeid(std::shared_ptr<WhileNode>)) \
    { \
        receiver = std::any_cast<std::shared_ptr<WhileNode>>(sender); \
    } \
    \
    else if (sender.type() == typeid(std::shared_ptr<DoNode>)) \
    { \
        receiver = std::any_cast<std::shared_ptr<DoNode>>(sender); \
    } \
    \
    else if (sender.type() == typeid(std::shared_ptr<FuncDefNode>)) \
    { \
        receiver = std::any_cast<std::shared_ptr<FuncDefNode>>(sender); \
    } \
    \
    else if (sender.type() == typeid(std::shared_ptr<CallNode>)) \
    { \
        receiver = std::any_cast<std::shared_ptr<CallNode>>(sender); \
    } \
    \
    else \
    { \
        throw WrongAny(); \
    } \

#define ANY_CAST_VECTOR(any_data, converted_error) \
    if (any_data.type() == typeid(std::shared_ptr<NumberNode>)) \
    { \
        converted_vector.push_back(std::any_cast<std::shared_ptr<NumberNode>>(any_data)); \
    } \
    \
    else if (any_data.type() == typeid(std::shared_ptr<StringNode>)) \
    { \
        converted_vector.push_back(std::any_cast<std::shared_ptr<StringNode>>(any_data)); \
    } \
    \
    else if (any_data.type() == typeid(std::shared_ptr<ListNode>)) \
    { \
        converted_vector.push_back(std::any_cast<std::shared_ptr<ListNode>>(any_data)); \
    } \
    \
    else if (any_data.type() == typeid(std::shared_ptr<VarAccessNode>)) \
    { \
        converted_vector.push_back(std::any_cast<std::shared_ptr<VarAccessNode>>(any_data)); \
    } \
    \
    else if (any_data.type() == typeid(std::shared_ptr<VarAssignNode>)) \
    { \
        converted_vector.push_back(std::any_cast<std::shared_ptr<VarAssignNode>>(any_data)); \
    } \
    \
    else if (any_data.type() == typeid(std::shared_ptr<BinOpNode>)) \
    { \
        converted_vector.push_back(std::any_cast<std::shared_ptr<BinOpNode>>(any_data)); \
    } \
    \
    else if (any_data.type() == typeid(std::shared_ptr<UnaryOpNode>)) \
    { \
        converted_vector.push_back(std::any_cast<std::shared_ptr<UnaryOpNode>>(any_data)); \
    } \
    \
    else if (any_data.type() == typeid(std::shared_ptr<IfNode>)) \
    { \
        converted_vector.push_back(std::any_cast<std::shared_ptr<IfNode>>(any_data)); \
    } \
    \
    else if (any_data.type() == typeid(std::shared_ptr<WhileNode>)) \
    { \
        converted_vector.push_back(std::any_cast<std::shared_ptr<WhileNode>>(any_data)); \
    } \
    \
    else if (any_data.type() == typeid(std::shared_ptr<DoNode>)) \
    { \
        converted_vector.push_back(std::any_cast<std::shared_ptr<DoNode>>(any_data)); \
    } \
    \
    else if (any_data.type() == typeid(std::shared_ptr<FuncDefNode>)) \
    { \
        converted_vector.push_back(std::any_cast<std::shared_ptr<FuncDefNode>>(any_data)); \
    } \
    \
    else if (any_data.type() == typeid(std::shared_ptr<CallNode>)) \
    { \
        converted_vector.push_back(std::any_cast<std::shared_ptr<CallNode>>(any_data)); \
    } \

NumberNode::NumberNode(Token tok) : tok(tok)
{
    this->tok = tok;

    this->pos_start = this->tok.pos_start;
    this->pos_end = this->tok.pos_end;
}

std::string NumberNode::repr()
{
    return tok.repr();
}

/* ---------------------------------------------------------------------------- */

StringNode::StringNode(Token tok) : tok(tok)
{
    this->tok = tok;

    this->pos_start = this->tok.pos_start;
    this->pos_end = this->tok.pos_end;
}

std::string StringNode::repr()
{
    return tok.repr();
}

/* ---------------------------------------------------------------------------- */

ListNode::ListNode(std::vector<std::any> element_nodes, std::shared_ptr<Position> pos_start, std::shared_ptr<Position> pos_end)
{
    std::vector<ALL_VARIANT> converted_vector;

    for (const auto& data : std::any_cast<std::vector<std::any>>(element_nodes)) 
    {
        std::any any_data = data;
        
        try {
            ANY_CAST_VECTOR(any_data, converted_vector); /* We do not need `else` stuff because we already have `catch` */
        } catch (const std::bad_any_cast& e) {
            throw ConvertError();
        }
    }

    this->element_nodes = converted_vector;

    this->pos_start = pos_start;
    this->pos_end = pos_end;
}

/* ---------------------------------------------------------------------------- */

VarAccessNode::VarAccessNode(Token var_name_tok) : var_name_tok(TT::NUL)
{
    this->var_name_tok = var_name_tok;

    this->pos_start = this->var_name_tok.pos_start;
    this->pos_end = this->var_name_tok.pos_end;
}

VarAccessNode::~VarAccessNode()
{

}

/* ---------------------------------------------------------------------------- */

VarAssignNode::VarAssignNode(Token var_name_tok, std::any value_node) : var_name_tok(TT::NUL)
{
    this->var_name_tok = var_name_tok;
    ANY_CAST(value_node, this->value_node);

    this->pos_start = this->var_name_tok.pos_start;
    this->pos_end = std::visit([](auto&& arg) { return arg->pos_end; }, this->value_node);
}

VarAssignNode::~VarAssignNode()
{

}

/* ---------------------------------------------------------------------------- */

BinOpNode::BinOpNode(std::any left_node, Token op_tok, std::any right_node) : op_tok(op_tok)
{
    ANY_CAST(left_node, this->left_node);
    this->op_tok = op_tok;
    ANY_CAST(right_node, this->right_node);

    if (std::holds_alternative<std::shared_ptr<UnaryOpNode>>(this->left_node))
    {
        this->pos_start = std::visit([](auto&& arg) { return arg->pos_start; }, this->left_node);
    }

    if (std::holds_alternative<std::shared_ptr<UnaryOpNode>>(this->right_node))
    {
        this->pos_end = std::visit([](auto&& arg) { return arg->pos_end; }, this->right_node);
    }
}

std::string BinOpNode::repr()
{
    return "TODO";
}

/* ---------------------------------------------------------------------------- */

UnaryOpNode::UnaryOpNode(Token op_tok, std::any node) : op_tok(op_tok)
{
    this->op_tok = op_tok;
    ANY_CAST(node, this->node);

    this->pos_start = this->op_tok.pos_start;
    this->pos_end = this->op_tok.pos_end;
}

UnaryOpNode::~UnaryOpNode()
{

}

std::string repr()
{
    return "TODO";
}

/* ---------------------------------------------------------------------------- */

IfNode::IfNode(std::any cases, std::any else_case)
{
    std::cout << cases.type().name() << std::endl;
    this->cases = std::any_cast<std::pair<std::vector<std::tuple<ALL_VARIANT, ALL_VARIANT, bool>>, std::any>>(cases);
    
    if (!else_case.has_value()) { this->is_else_case_empty = true; }

    if (else_case.type() == typeid(std::pair<ALL_VARIANT, bool>)) 
    { 
        this->else_case = std::any_cast<std::pair<ALL_VARIANT, bool>>(else_case);
    }

    else
    {
        this->is_else_case_empty = true;
    }

    this->pos_start = std::visit([](auto&& arg) { return arg->pos_start; }, std::get<0>(this->cases.first.at(0)));

    if (this->is_else_case_empty) { this->pos_end = std::visit([](auto&& arg) { return arg->pos_end; }, std::get<1>(this->cases.first.at(0))); }
    else { this->pos_end = std::visit([](auto&& arg) { return arg->pos_end; }, this->else_case.first); }
}

IfNode::~IfNode()
{

}

/* ---------------------------------------------------------------------------- */

ForNode::ForNode(Token var_name_tok, std::any start_value_node, std::any end_value_node, std::any step_value_node, std::any body_node, bool should_return_null) : var_name_tok(TT::NUL)
{
    this->var_name_tok = var_name_tok;

    ANY_CAST(start_value_node, this->start_value_node);
    ANY_CAST(end_value_node, this->end_value_node);
    ANY_CAST(step_value_node, this->step_value_node);
    ANY_CAST(body_node, this->body_node);

    this->should_return_null = should_return_null;

    this->pos_start = this->var_name_tok.pos_start;
    this->pos_end = std::visit([](auto&& arg) { return arg->pos_end; }, this->body_node);
}

ForNode::~ForNode()
{

}

/* ---------------------------------------------------------------------------- */

WhileNode::WhileNode(std::any condition_node, std::any body_node, bool should_return_null)
{
    ANY_CAST(condition_node, this->condition_node);
    ANY_CAST(body_node, this->body_node);

    this->should_return_null = should_return_null;

    this->pos_start = std::visit([](auto&& arg) { return arg->pos_start; }, this->condition_node);
    this->pos_end = std::visit([](auto&& arg) { return arg->pos_end; }, this->body_node);
}

WhileNode::~WhileNode()
{

}

/* ---------------------------------------------------------------------------- */

DoNode::DoNode(std::any body_node, std::any condition_node, bool should_return_null)
{
    ANY_CAST(body_node, this->body_node);
    ANY_CAST(condition_node, this->condition_node);

    this->should_return_null = should_return_null;

    this->pos_start = std::visit([](auto&& arg) { return arg->pos_start; }, this->body_node);
    this->pos_end = std::visit([](auto&& arg) { return arg->pos_end; }, this->condition_node);
}

DoNode::~DoNode()
{

}

/* ---------------------------------------------------------------------------- */

FuncDefNode::FuncDefNode(Token var_name_tok, std::vector<Token> arg_name_toks, std::any body_node, bool should_return_null) : var_name_tok(TT::NUL)
{
    this->var_name_tok = var_name_tok;
    this->arg_name_toks = arg_name_toks;
    ANY_CAST(body_node, this->body_node);
    this->should_return_null = should_return_null;

    if (this->var_name_tok.type != TT::NUL) { this->pos_start = this->var_name_tok.pos_start; }
    else if (this->arg_name_toks.size() > 0) { this->pos_start = this->arg_name_toks.at(0).pos_start; }
    else {this->pos_start = std::visit([](auto&& arg) { return arg->pos_start; }, this->body_node); }

    this->pos_end = std::visit([](auto&& arg) { return arg->pos_end; }, this->body_node);
}

FuncDefNode::~FuncDefNode()
{

}

/* ---------------------------------------------------------------------------- */

CallNode::CallNode(std::any node_to_call, std::vector<std::any> arg_nodes)
{
    ANY_CAST(node_to_call, this->node_to_call);

    std::vector<ALL_VARIANT> converted_vector;

    for (const auto& data : std::any_cast<std::vector<std::any>>(arg_nodes)) {
        std::any any_data = data;
        
        try {
            ANY_CAST_VECTOR(any_data, converted_vector); /* We do not need `else` stuff because we already have `catch` */
        } catch (const std::bad_any_cast& e) {
            throw ConvertError();
        }
    }

    this->arg_nodes = converted_vector;

    this->pos_start = std::visit([](auto&& arg) { return arg->pos_start; }, this->node_to_call);

    if (this->arg_nodes.size() > 0) { this->pos_end = std::visit([](auto&& arg) { return arg->pos_start; }, this->arg_nodes.at(this->arg_nodes.size() - 1)); }
    else { this->pos_end = std::visit([](auto&& arg) { return arg->pos_end; }, this->node_to_call); }
}

CallNode::~CallNode()
{

}
