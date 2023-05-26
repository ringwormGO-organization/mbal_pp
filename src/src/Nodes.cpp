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

BinOpNode::BinOpNode(NumberNode left_node, Token op_tok, NumberNode right_node) : left_node(left_node), op_tok(op_tok), right_node(right_node)
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

UnaryOpNode::UnaryOpNode(Token op_tok, std::variant<std::monostate, NumberNode, BinOpNode> node) : op_tok(op_tok), node(std::move(node))
{
    this->op_tok = op_tok;
}

UnaryOpNode::~UnaryOpNode()
{

}

std::string repr()
{

}
