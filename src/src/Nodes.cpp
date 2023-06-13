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

VarAssignNode::VarAssignNode(Token var_name_tok, std::shared_ptr<NumberNode> value_node) : var_name_tok(TT::NUL)
{
    this->var_name_tok = var_name_tok;
    this->value_node = value_node;

    this->pos_start = this->var_name_tok.pos_start;
    this->pos_end = this->value_node->pos_end;
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
        this->pos_start = std::get<2>(this->left_node).get()->pos_start;
    }

    if (std::holds_alternative<std::shared_ptr<UnaryOpNode>>(this->right_node))
    {
        this->pos_end = std::get<2>(this->right_node).get()->pos_start;
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
