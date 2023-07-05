/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Nodes.hpp"

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

ListNode::ListNode(std::vector<ALL_VARIANT> element_nodes, std::shared_ptr<Position> pos_start, std::shared_ptr<Position> pos_end)
{
    this->element_nodes = element_nodes;

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

VarAssignNode::VarAssignNode(Token var_name_tok, ALL_VARIANT value_node) : var_name_tok(TT::NUL)
{
    this->var_name_tok = var_name_tok;
    this->value_node = value_node;

    this->pos_start = this->var_name_tok.pos_start;
    this->pos_end = std::visit([](auto&& arg) { return arg->pos_end; }, this->value_node);
}

VarAssignNode::~VarAssignNode()
{

}

/* ---------------------------------------------------------------------------- */

BinOpNode::BinOpNode(ALL_VARIANT left_node, Token op_tok, ALL_VARIANT right_node) : op_tok(op_tok)
{
    this->left_node = left_node;
    this->op_tok = op_tok;
    this->right_node = right_node;

    if (std::holds_alternative<std::shared_ptr<UnaryOpNode>>(this->left_node))
    {
        this->pos_start = std::visit([](auto&& arg) { return arg->pos_start; }, left_node);
    }

    if (std::holds_alternative<std::shared_ptr<UnaryOpNode>>(this->right_node))
    {
        this->pos_end = std::visit([](auto&& arg) { return arg->pos_end; }, right_node);
    }
}

std::string BinOpNode::repr()
{
    return "TODO";
}

/* ---------------------------------------------------------------------------- */

UnaryOpNode::UnaryOpNode(Token op_tok, ALL_VARIANT node) : op_tok(op_tok)
{
    this->op_tok = op_tok;
    this->node = node;

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

IfNode::IfNode(std::vector<std::pair<ALL_VARIANT, ALL_VARIANT>> cases, ALL_VARIANT else_case)
{
    this->cases = cases;
    this->else_case = else_case;

    this->pos_start = std::visit([](auto&& arg) { return arg->pos_start; }, cases.at(0).first);

    bool is_empty = (else_case == ALL_VARIANT{});
    if (is_empty) { this->pos_end = std::visit([](auto&& arg) { return arg->pos_end; }, cases.at(cases.size() - 1).first); }
    else { this->pos_end =  std::visit([](auto&& arg) { return arg->pos_end; }, else_case); }
}

IfNode::~IfNode()
{

}

/* ---------------------------------------------------------------------------- */

ForNode::ForNode(Token var_name_tok, ALL_VARIANT start_value_node, ALL_VARIANT end_value_node, ALL_VARIANT step_value_node, ALL_VARIANT body_node) : var_name_tok(TT::NUL)
{
    this->var_name_tok = var_name_tok;
    this->start_value_node = start_value_node;
    this->end_value_node = end_value_node;
    this->step_value_node = step_value_node;
    this->body_node = body_node;

    this->pos_start = this->var_name_tok.pos_start;
    this->pos_end = std::visit([](auto&& arg) { return arg->pos_end; }, this->body_node);
}

ForNode::~ForNode()
{

}

/* ---------------------------------------------------------------------------- */

WhileNode::WhileNode(ALL_VARIANT condition_node, ALL_VARIANT body_node)
{
    this->condition_node = condition_node;
    this->body_node = body_node;

    this->pos_start = std::visit([](auto&& arg) { return arg->pos_start; }, this->condition_node);
    this->pos_end = std::visit([](auto&& arg) { return arg->pos_end; }, this->body_node);
}

WhileNode::~WhileNode()
{

}

/* ---------------------------------------------------------------------------- */

DoNode::DoNode(ALL_VARIANT body_node, ALL_VARIANT condition_node)
{
    this->body_node = body_node;
    this->condition_node = condition_node;

    this->pos_start = std::visit([](auto&& arg) { return arg->pos_start; }, this->body_node);
    this->pos_end = std::visit([](auto&& arg) { return arg->pos_end; }, this->condition_node);
}

DoNode::~DoNode()
{

}

/* ---------------------------------------------------------------------------- */

FuncDefNode::FuncDefNode(Token var_name_tok, std::vector<Token> arg_name_toks, ALL_VARIANT body_node) : var_name_tok(TT::NUL)
{
    this->var_name_tok = var_name_tok;
    this->arg_name_toks = arg_name_toks;
    this->body_node = body_node;

    if (this->var_name_tok.type != TT::NUL) { this->pos_start = this->var_name_tok.pos_start; }
    else if (this->arg_name_toks.size() > 0) { this->pos_start = this->arg_name_toks.at(0).pos_start; }
    else {this->pos_start = std::visit([](auto&& arg) { return arg->pos_start; }, this->body_node); }

    this->pos_end = std::visit([](auto&& arg) { return arg->pos_end; }, this->body_node);
}

FuncDefNode::~FuncDefNode()
{

}

/* ---------------------------------------------------------------------------- */

CallNode::CallNode(ALL_VARIANT node_to_call, std::vector<ALL_VARIANT> arg_nodes)
{
    this->node_to_call = node_to_call;
    this->arg_nodes = arg_nodes;

    this->pos_start = std::visit([](auto&& arg) { return arg->pos_start; }, this->node_to_call);

    if (this->arg_nodes.size() > 0) { this->pos_end = std::visit([](auto&& arg) { return arg->pos_start; }, this->arg_nodes.at(this->arg_nodes.size() - 1)); }
    else { this->pos_end = std::visit([](auto&& arg) { return arg->pos_end; }, this->node_to_call); }
}

CallNode::~CallNode()
{

}
