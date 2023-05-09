/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Parser.hpp"

Parser::Parser(std::vector<Token> tokens)
{
    this->tokens = tokens;
    this->advance();
}

Parser::~Parser()
{

}

Token Parser::advance()
{
    this->tok_idx += 1;

    if (this->tok_idx < this->tokens.size())
    {
        current_tok = this->tokens[this->tok_idx];
    }

    return current_tok;
}

std::variant<NumberNode, BinOpNode> Parser::parse()
{
    std::variant<NumberNode, BinOpNode> res = this->expr();
    return res;
}

NumberNode Parser::factor()
{
    Token tok = current_tok;

    if (tok.type == TT::INT || tok.type == TT::FLOAT)
    {
        this->advance();
        return NumberNode(tok);
    }
}

std::variant<NumberNode, BinOpNode> Parser::term()
{
    TT ops[2] {TT::MUL, TT::DIV};

    std::function<std::variant<NumberNode, BinOpNode>()> fac = [this]() { return this->factor(); };
    return this->bin_op<2>(fac, ops);
}

std::variant<NumberNode, BinOpNode> Parser::expr()
{
    TT ops[2] {TT::PLUS, TT::MINUS};

    std::function<std::variant<NumberNode, BinOpNode>()> fac = [this]() { return this->factor(); };
    return this->bin_op<2>(fac, ops);
}

template <int N>
std::variant<NumberNode, BinOpNode> Parser::bin_op(std::function<std::variant<NumberNode, BinOpNode>()> func, TT ops[N])
{
    std::variant<NumberNode, BinOpNode> left = std::get<NumberNode>(func());
    
    while (this->contains<2>(current_tok.type, ops))
    {
        Token op_tok = current_tok;
        this->advance();

        NumberNode right = std::get<NumberNode>(func());
        left = BinOpNode(std::get<NumberNode>(left), op_tok, right);
    }

    /*std::cout << std::boolalpha
              << "NumberNode? "
              << std::holds_alternative<NumberNode>(left) << '\n'
              << "BinOpNode? "
              << std::holds_alternative<BinOpNode>(left) << '\n';*/

    return left;
}

template <int N>
bool Parser::contains(TT current, TT ops[N])
{
    for (size_t i = 0; i < N; i++)
    {
        if (current == ops[i])
        {
            return true;
        }
    }

    return false;
}
