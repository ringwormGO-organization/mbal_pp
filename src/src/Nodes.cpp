/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Nodes.hpp"

NumberNode::NumberNode(Token tok) : tok(tok), pos_start(pos_start), pos_end(pos_end)
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

BinOpNode::BinOpNode(ALL_VARIANT left_node, Token op_tok, ALL_VARIANT right_node) : op_tok(op_tok), pos_start(pos_start), pos_end(pos_end)
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

UnaryOpNode::UnaryOpNode(Token op_tok, ALL_VARIANT node) : op_tok(op_tok), pos_start(pos_start), pos_end(pos_end)
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
