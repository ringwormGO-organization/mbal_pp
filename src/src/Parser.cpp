/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Parser.hpp"

std::variant<Token, ParseResult, std::variant<NumberNode, BinOpNode>> ParseResult::register_result(std::variant<Token, ParseResult, std::variant<NumberNode, BinOpNode>> res)
{
    if (std::holds_alternative<ParseResult>(res))
    {
        if (std::get<ParseResult>(res).error.error_name != "")
        {
            this->error = std::get<ParseResult>(res).error;
        }

        return std::get<ParseResult>(res).node;
    }

    return res;
}

ParseResult& ParseResult::success(std::variant<NumberNode, BinOpNode> node)
{
    this->node = node;
    return *this;
}

ParseResult& ParseResult::failure(Error error)
{
    this->error = error;
    return *this;
}

/* ---------------------------------------------------------------------------- */

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

ParseResult Parser::parse()
{
    ParseResult res = this->expr();

    if (res.error.error_name == "" && current_tok.type != TT::END_OF_FILE)
    {
        return res.failure (
            InvalidSyntaxError (
                current_tok.pos_start, current_tok.pos_end,
                "Expected '+', '-', '*'or '/'"
            )
        );
    }

    return res;
}

ParseResult Parser::factor()
{
    ParseResult res = ParseResult();
    Token tok = current_tok;

    if (tok.type == TT::INT || tok.type == TT::FLOAT)
    {
        res.register_result(this->advance());
        return res.success(NumberNode(tok));
    }

    return res.failure(InvalidSyntaxError (
        tok.pos_start, tok.pos_end,
        "Expected INT or FLOAT"
    ));
}

ParseResult Parser::term()
{
    TT ops[2] {TT::MUL, TT::DIV};

    std::function<ParseResult()> fac = [this]() { return this->factor(); };
    return this->bin_op<2>(fac, ops);
}

ParseResult Parser::expr()
{
    TT ops[2] {TT::PLUS, TT::MINUS};

    std::function<ParseResult()> fac = [this]() { return this->term(); };
    return this->bin_op<2>(fac, ops);
}

template <int N>
ParseResult Parser::bin_op(std::function<ParseResult()> func, TT ops[N])
{
    ParseResult res = ParseResult();
    auto left = res.register_result(func());

    if (res.error.error_name != "") return res;
    while (this->contains<N>(current_tok.type, ops))
    {
        Token op_tok = current_tok;
        res.register_result(this->advance());

        auto right = res.register_result(func());
        if (res.error.error_name != "") return res;

        left = BinOpNode (
            std::get<NumberNode>(std::get<std::variant<NumberNode, BinOpNode>>(left)),
            op_tok, 
            std::get<NumberNode>(std::get<std::variant<NumberNode, BinOpNode>>(right))
        );
    }

    return res.success(std::get<2>(left));
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
