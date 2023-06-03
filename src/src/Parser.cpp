/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Parser.hpp"

ParseResult::ParseResult() : error(EMPTY_POSITION, EMPTY_POSITION, "", ""), node()
{

}

ParseResult::~ParseResult()
{

}

PARSE_REGISTER_TYPES ParseResult::register_result(PARSE_REGISTER_TYPES res)
{
    if (std::holds_alternative<std::shared_ptr<ParseResult>>(res))
    {
        if (std::get<std::shared_ptr<ParseResult>>(res).get()->error.error_name != "")
        {
            this->error = std::get<std::shared_ptr<ParseResult>>(res).get()->error;
        }

        return std::get<std::shared_ptr<ParseResult>>(res).get()->node;
    }

    return res;
}

std::shared_ptr<ParseResult> ParseResult::success(ALL_VARIANT node)
{
    this->node = node;
    return std::shared_ptr<ParseResult>(shared_from_this());
}

std::shared_ptr<ParseResult> ParseResult::failure(Error error)
{
    this->error = error;
    return std::shared_ptr<ParseResult>(shared_from_this());
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

std::shared_ptr<ParseResult> Parser::parse()
{
    std::shared_ptr<ParseResult> res = this->expr();

    if (res.get()->error.error_name == "" && current_tok.type != TT::END_OF_FILE)
    {
        return res.get()->failure (
            InvalidSyntaxError (
                current_tok.pos_start, current_tok.pos_end,
                "Expected '+', '-', '*'or '/'"
            )
        );
    }

    return res;
}

std::shared_ptr<ParseResult> Parser::factor()
{
    std::shared_ptr<ParseResult> res = std::make_shared<ParseResult>();
    Token tok = current_tok;

    if (tok.type == TT::PLUS || tok.type == TT::MINUS)
    {
        res.get()->register_result(this->advance());
        auto factor = res.get()->register_result(this->factor());

        if (res.get()->error.error_name != "")
        {
            return res;
        }

        return res.get()->success(std::make_shared<UnaryOpNode>(tok, std::get<2>(factor)));
    }

    else if (tok.type == TT::INT || tok.type == TT::FLOAT)
    {
        res.get()->register_result(this->advance());
        return res->success(std::make_shared<NumberNode>(tok));
    }

    else if (tok.type == TT::LPAREN)
    {
        res.get()->register_result(this->advance());
        auto expr = res.get()->register_result(this->expr());

        if (res.get()->error.error_name != "")
        {
            return res;
        }

        if (current_tok.type == TT::RPAREN)
        {
            res.get()->register_result(this->advance());
            return res.get()->success(std::get<2>(expr));
        }

        else
        {
            return res.get()->failure(InvalidSyntaxError(
                current_tok.pos_start, current_tok.pos_end,
                "Expected ')'"
            ));
        }
    }

    return res.get()->failure(InvalidSyntaxError(
        tok.pos_start, tok.pos_end,
        "Expected INT or FLOAT"
    ));
}

std::shared_ptr<ParseResult> Parser::term()
{
    TT ops[2] {TT::MUL, TT::DIV};

    std::function<std::shared_ptr<ParseResult>()> fac = [this]() { return this->factor(); };
    return this->bin_op<2>(fac, ops);
}

std::shared_ptr<ParseResult> Parser::expr()
{
    TT ops[2] {TT::PLUS, TT::MINUS};

    std::function<std::shared_ptr<ParseResult>()> fac = [this]() { return this->term(); };
    return this->bin_op<2>(fac, ops);
}

template <int N>
std::shared_ptr<ParseResult> Parser::bin_op(std::function<std::shared_ptr<ParseResult>()> func, TT ops[N])
{
    std::shared_ptr<ParseResult> res = std::make_shared<ParseResult>();
    auto left = res.get()->register_result(func());

    if (res.get()->error.error_name != "") return res;
    while (this->contains<N>(current_tok.type, ops))
    {
        Token op_tok = current_tok;
        res.get()->register_result(this->advance());

        auto right = res.get()->register_result(func());
        if (res.get()->error.error_name != "") return res;

        left = std::make_shared<BinOpNode> (
            std::get<ALL_VARIANT>(left),
            op_tok,
            std::get<ALL_VARIANT>(right)
        );
    }

    return res.get()->success(std::get<ALL_VARIANT>(left));
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
