/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Nodes.hpp"

NumberNode::NumberNode(Token tok) : tok(tok)
{
    this->tok = tok;
}

std::string NumberNode::repr()
{
    return tok.repr();
}

/* ---------------------------------------------------------------------------- */

BinOpNode::BinOpNode(std::variant<NumberNode*, BinOpNode*, UnaryOpNode*> left_node, Token op_tok, std::variant<NumberNode*, BinOpNode*, UnaryOpNode*> right_node) : op_tok(op_tok)
{
    this->left_node = left_node;
    this->op_tok = op_tok;
    this->right_node = right_node;
}

std::string BinOpNode::repr()
{
    return "TODO";
}

/* ---------------------------------------------------------------------------- */

UnaryOpNode::UnaryOpNode(Token op_tok, std::variant<NumberNode*, BinOpNode*, UnaryOpNode*> node) : op_tok(op_tok), node(std::move(node))
{
    this->op_tok = op_tok;
    this->node = node;
}

UnaryOpNode::~UnaryOpNode()
{

}

std::string repr()
{
    return "TODO";
}
