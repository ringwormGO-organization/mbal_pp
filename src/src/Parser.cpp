/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Parser.hpp"

ParseResult::ParseResult()
{

}

ParseResult::~ParseResult()
{

}

void ParseResult::register_advancement()
{
    this->advance_count++;
}

ALL_VARIANT ParseResult::register_result(std::shared_ptr<ParseResult> res)
{
    this->advance_count += res->advance_count;

    if (res->error->error_name != "")
    {
        this->error = res->error;
    }

    return res->node;
}

std::shared_ptr<ParseResult> ParseResult::success(ALL_VARIANT node)
{
    this->node = node;
    return std::shared_ptr<ParseResult>(shared_from_this());
}

std::shared_ptr<ParseResult> ParseResult::failure(std::shared_ptr<Error> error)
{
    if (this->error->error_name == "" && this->advance_count == 0) /* there is no error and therefore error_name is empty */
    {
        this->error = error;
    }

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
    if (this->first_run)
    {
        first_run = false;
    }

    else
    {
        this->tok_idx += 1;
    }

    if (this->tok_idx < this->tokens.size())
    {
        current_tok = this->tokens[this->tok_idx];
    }

    return current_tok;
}

std::shared_ptr<ParseResult> Parser::parse()
{
    std::shared_ptr<ParseResult> res = this->expr();

    if (res->error->error_name == "" && current_tok.type != TT::END_OF_FILE)
    {
        return res->failure (
            std::make_shared<InvalidSyntaxError> (
                current_tok.pos_start, current_tok.pos_end,
                "Expected '+', '-', '*' or '/'"
            )
        );
    }

    return res;
}

std::shared_ptr<ParseResult> Parser::atom()
{
    std::shared_ptr<ParseResult> res = std::make_shared<ParseResult>();
    Token tok = current_tok;

    if (tok.type == TT::INT || tok.type == TT::FLOAT)
    {
        res->register_advancement();
        this->advance();

        return res->success(std::make_shared<NumberNode>(tok));
    }

    else if (tok.type == TT::IDENTIFIER)
    {
        res->register_advancement();
        this->advance();

        return res->success(std::make_shared<VarAccessNode>(tok));
    }

    else if (tok.type == TT::LPAREN)
    {
        res->register_advancement();
        this->advance();

        auto expr = res->register_result(this->expr());

        if (res->error->error_name != "")
        {
            return res;
        }

        if (current_tok.type == TT::RPAREN)
        {
            res->register_advancement();
            this->advance();

            return res->success(std::get<2>(expr));
        }

        else
        {
            return res->failure(std::make_shared<InvalidSyntaxError> (
                current_tok.pos_start, current_tok.pos_end,
                "Expected ')'"
            ));
        }
    }

    return res->failure(std::make_shared<InvalidSyntaxError>(
        tok.pos_start, tok.pos_end,
        "Expected INT or FLOAT, identifier, 'VAR', '+', '-' or '('"
    ));
}

std::shared_ptr<ParseResult> Parser::power()
{
    TT ops[1] {TT::POW};

    std::function<std::shared_ptr<ParseResult>()> fac_a = [this]() { return this->atom(); };
    std::function<std::shared_ptr<ParseResult>()> fac_b = [this]() { return this->factor(); };

    return this->bin_op<1>(fac_a, ops, fac_b);
}

std::shared_ptr<ParseResult> Parser::factor()
{
    std::shared_ptr<ParseResult> res = std::make_shared<ParseResult>();
    Token tok = current_tok;

    if (tok.type == TT::PLUS || tok.type == TT::MINUS)
    {
        res->register_advancement();
        this->advance();

        auto factor = res->register_result(this->factor());
        if (res->error->error_name != "")
        {
            return res;
        }

        return res->success(std::make_shared<UnaryOpNode>(tok, std::get<2>(factor)));
    }

    return this->power();
}

std::shared_ptr<ParseResult> Parser::term()
{
    TT ops[2] {TT::MUL, TT::DIV};

    std::function<std::shared_ptr<ParseResult>()> fac = [this]() { return this->factor(); };
    return this->bin_op<2>(fac, ops);
}

std::shared_ptr<ParseResult> Parser::expr()
{
    std::shared_ptr<ParseResult> res = std::make_shared<ParseResult>();

    if (current_tok.matches(TT::KEYWORD, "VAR"))
    {
        res->register_advancement();
        this->advance();
        
        if (current_tok.type != TT::IDENTIFIER)
        {
            return res->failure(std::make_shared<InvalidSyntaxError> (
                current_tok.pos_start, current_tok.pos_end,
                "Expected an identifier"
            ));
        }

        Token var_name = current_tok;

        res->register_advancement();
        this->advance();

        if (current_tok.type != TT::EQ)
        {
            return res->failure(std::make_shared<InvalidSyntaxError> (
                current_tok.pos_start, current_tok.pos_end,
                "Expected '='"
            ));
        }

        res->register_advancement();
        this->advance();

        auto expression = res->register_result(this->expr());

        if (res->error->error_name != "") { return res; }
        return res->success(std::make_shared<VarAssignNode>(var_name, std::get<std::shared_ptr<NumberNode>>(expression)));
    }

    TT ops[2] {TT::PLUS, TT::MINUS};
    std::function<std::shared_ptr<ParseResult>()> fac = [this]() { return this->term(); };
    
    ALL_VARIANT node = res->register_result(this->bin_op<2>(fac, ops));
    if (res->error->error_name != "") 
    {
        return res->failure(std::make_shared<InvalidSyntaxError> (
            current_tok.pos_start, current_tok.pos_end,
            "Expected 'VAR', int, float, identifier, '+', '-', or '('"
        )); 
    }

    return res->success(node);
}

template <int N>
std::shared_ptr<ParseResult> Parser::bin_op(std::function<std::shared_ptr<ParseResult>()> func_a, TT ops[N], std::function<std::shared_ptr<ParseResult>()> func_b)
{
    if (func_b == nullptr)
    {
        func_b = func_a;
    }

    std::shared_ptr<ParseResult> res = std::make_shared<ParseResult>();
    auto left = res->register_result(func_a());

    if (res->error->error_name != "") return res;
    while (this->contains<N>(current_tok.type, ops))
    {
        Token op_tok = current_tok;
        
        res->register_advancement();
        this->advance();

        auto right = res->register_result(func_b());
        if (res->error->error_name != "") return res;

        left = std::make_shared<BinOpNode> (
            left,
            op_tok,
            right
        );
    }

    return res->success(left);
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
