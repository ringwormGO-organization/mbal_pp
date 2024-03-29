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

ALL_VARIANT ParseResult::try_register(std::shared_ptr<ParseResult> res)
{
    if (res->error->error_name != "")
    {
        this->to_reverse_count = res->advance_count;
        return std::make_shared<NumberNode>(TT::NUL);
    }

    return this->register_result(res);
}

std::shared_ptr<ParseResult> ParseResult::success(ALL_VARIANT node)
{
    this->node = node;
    return std::shared_ptr<ParseResult>(shared_from_this());
}

std::shared_ptr<ParseResult> ParseResult::failure(std::shared_ptr<Error> error)
{
    if (this->error->error_name == "" || this->advance_count == 0) /* there is no error and therefore error_name is empty */
    {
        this->error = error;
    }

    return std::shared_ptr<ParseResult>(shared_from_this());
}

/* ---------------------------------------------------------------------------- */

Parser::Parser(std::vector<Token> tokens) : current_tok(TT::NUL)
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
        this->first_run = false;
    }

    else
    {
        this->tok_idx += 1;
    }

    if (this->tok_idx < this->tokens.size())
    {
        this->current_tok = this->tokens[this->tok_idx];
    }

    return this->current_tok;
}

Token Parser::reverse(size_t amount)
{
    this->tok_idx -= amount;
    this->update_current_tok();

    return this->current_tok;
}

void Parser::update_current_tok()
{
    if (this->tok_idx < this->tokens.size())
    {
        this->current_tok = this->tokens.at(this->tok_idx);
    }
}

std::shared_ptr<ParseResult> Parser::parse()
{
    std::shared_ptr<ParseResult> res = this->statements();

    if (res->error->error_name == "" && this->current_tok.type != TT::END_OF_FILE)
    {
        return res->failure (
            std::make_shared<InvalidSyntaxError> (
                this->current_tok.pos_start, this->current_tok.pos_end,
                "Expected '+', '-', '*', '/', '^', '==', '!=', '<', '>', <=', '>=', 'AND' or 'OR'"
            )
        );
    }

    return res;
}

/* ------------------------------------ */

std::shared_ptr<ParseResult> Parser::statements()
{
    std::shared_ptr<ParseResult> res = std::make_shared<ParseResult>();

    std::vector<ALL_VARIANT> statements;
    std::shared_ptr<Position> pos_start = this->current_tok.pos_start->copy();

    while (this->current_tok.type == TT::NEWLINE)
    {
        res->register_advancement();
        this->advance();
    }

    ALL_VARIANT statement = res->register_result(this->expr());
    if (res->error->error_name != "") { return res; }
    statements.push_back(statement);

    bool more_statements = true;

    while (true)
    {
        unsigned int newline_count = 0;

        while (this->current_tok.type == TT::NEWLINE)
        {
            res->register_advancement();
            this->advance();

            newline_count += 1;
        }

        if (newline_count == 0)
        {
            more_statements = false;
        }

        if (!more_statements) { break; }
        statement = res->try_register(this->expr());

        if (std::holds_alternative<std::shared_ptr<NumberNode>>(statement)) /* variant is holding monostate empty which means it's empty (!statement) */
        {
            if (std::get<0>(statement)->tok.type == TT::NUL)
            {
                this->reverse(res->to_reverse_count);
                more_statements = false;
                continue;
            }
        }

        statements.push_back(statement);
    }

    return res->success(std::make_shared<ListNode>(
        statements,
        pos_start,
        this->current_tok.pos_end->copy()
    ));
}

std::shared_ptr<ParseResult> Parser::expr()
{
    std::shared_ptr<ParseResult> res = std::make_shared<ParseResult>();

    if (this->current_tok.matches(TT::KEYWORD, KEYWORDS[0]))
    {
        res->register_advancement();
        this->advance();
        
        if (this->current_tok.type != TT::IDENTIFIER)
        {
            return res->failure(std::make_shared<InvalidSyntaxError> (
                this->current_tok.pos_start, this->current_tok.pos_end,
                "Expected an identifier"
            ));
        }

        Token var_name = this->current_tok;

        res->register_advancement();
        this->advance();

        if (this->current_tok.type != TT::EQ)
        {
            return res->failure(std::make_shared<InvalidSyntaxError> (
                this->current_tok.pos_start, this->current_tok.pos_end,
                "Expected '='"
            ));
        }

        res->register_advancement();
        this->advance();

        auto expression = res->register_result(this->expr());
        if (res->error->error_name != "") { return res; }

        return res->success(std::make_shared<VarAssignNode>(var_name, expression));
    }

    std::vector<std::pair<TT, std::string>> ops = {
        std::make_pair(TT::KEYWORD, "AND"),
        std::make_pair(TT::KEYWORD, "OR"),
    };

    std::function<std::shared_ptr<ParseResult>()> fac = [this]() { return this->comp_expr(); };
    ALL_VARIANT node = res->register_result(this->bin_op(fac, ops));

    if (res->error->error_name != "") 
    {
        return res->failure(std::make_shared<InvalidSyntaxError> (
            this->current_tok.pos_start, this->current_tok.pos_end,
            "Expected 'LET', int, float, identifier, '+', '-', '(', '[' or 'NOT'"
        )); 
    }

    return res->success(node);
}

std::shared_ptr<ParseResult> Parser::comp_expr()
{
    std::shared_ptr<ParseResult> res = std::make_shared<ParseResult>();

    if (this->current_tok.matches(TT::KEYWORD, KEYWORDS[3]))
    {
        Token op_tok = this->current_tok;

        res->register_advancement();
        this->advance();

        ALL_VARIANT node = res->register_result(this->comp_expr());
        if (res->error->error_name != "") { return res; }

        return res->success(std::make_shared<UnaryOpNode>(op_tok, node));
    }

    std::vector<std::pair<TT, std::string>> ops = {
        std::make_pair(TT::EE, ""),
        std::make_pair(TT::NE, ""),
        std::make_pair(TT::LT, ""),
        std::make_pair(TT::GT, ""),
        std::make_pair(TT::LTE, ""),
        std::make_pair(TT::GTE, ""),
    };

    std::function<std::shared_ptr<ParseResult>()> fac = [this]() { return this->arith_expr(); };
    ALL_VARIANT node = res->register_result(this->bin_op(fac, ops));

    if (res->error->error_name != "") 
    { 
        return res->failure(std::make_shared<InvalidSyntaxError>(
            this->current_tok.pos_start, this->current_tok.pos_end,
            "Expected int, float, identifier, '+', '-', '(', '[' or 'NOT'"
        )); 
    }

    return res->success(node);
}

std::shared_ptr<ParseResult> Parser::arith_expr()
{
    std::vector<std::pair<TT, std::string>> ops = {
        std::make_pair(TT::PLUS, ""),
        std::make_pair(TT::MINUS, ""),
    };
    
    std::function<std::shared_ptr<ParseResult>()> fac = [this]() { return this->term(); };
    return this->bin_op(fac, ops);
}

std::shared_ptr<ParseResult> Parser::term()
{
    std::vector<std::pair<TT, std::string>> ops = {
        std::make_pair(TT::MUL, ""),
        std::make_pair(TT::DIV, ""),
    };

    std::function<std::shared_ptr<ParseResult>()> fac = [this]() { return this->factor(); };
    return this->bin_op(fac, ops);
}

std::shared_ptr<ParseResult> Parser::factor()
{
    std::shared_ptr<ParseResult> res = std::make_shared<ParseResult>();
    Token tok = this->current_tok;

    if (tok.type == TT::PLUS || tok.type == TT::MINUS)
    {
        res->register_advancement();
        this->advance();

        auto factor = res->register_result(this->factor());
        if (res->error->error_name != "") { return res; }

        return res->success(std::make_shared<UnaryOpNode>(tok, factor));
    }

    return this->power();
}

std::shared_ptr<ParseResult> Parser::power()
{
    std::vector<std::pair<TT, std::string>> ops = {
        std::make_pair(TT::POW, ""),
    };

    std::function<std::shared_ptr<ParseResult>()> fac = [this]() { return this->call(); };

    return this->bin_op(fac, ops);
}

std::shared_ptr<ParseResult> Parser::call()
{
    std::shared_ptr<ParseResult> res = std::make_shared<ParseResult>();

    ALL_VARIANT atom = res->register_result(this->atom());
    if (res->error->error_name != "") { return res; }

    if (this->current_tok.type == TT::LPAREN)
    {
        res->register_advancement();
        this->advance();

        std::vector<ALL_VARIANT> arg_nodes;

        if (this->current_tok.type == TT::RPAREN)
        {
            res->register_advancement();
            this->advance();
        }

        else
        {
            arg_nodes.push_back(res->register_result(this->expr()));
            if (res->error->error_name != "") 
            { 
                return res->failure(std::make_shared<InvalidSyntaxError> (
                    this->current_tok.pos_start, this->current_tok.pos_end,
                    "Expected ')', 'LET', 'IF', 'FOR', 'WHILE', 'DO', 'FN', int, float, identifier, '+', '-', '(', '[' or 'NOT'"
                ));
            }

            while (this->current_tok.type == TT::COMMA)
            {
                res->register_advancement();
                this->advance();

                arg_nodes.push_back(res->register_result(this->expr()));
                if (res->error->error_name != "") { return res; }
            }

            if (this->current_tok.type != TT::RPAREN)
            {
                return res->failure(std::make_shared<InvalidSyntaxError> (
                    this->current_tok.pos_start, this->current_tok.pos_end,
                    "Expected ',' or ')"
                ));
            }

            res->register_advancement();
            this->advance();
        }

        return res->success(std::make_shared<CallNode>(atom, arg_nodes));
    }

    return res->success(atom);
}

std::shared_ptr<ParseResult> Parser::atom()
{
    std::shared_ptr<ParseResult> res = std::make_shared<ParseResult>();
    Token tok = this->current_tok;

    if (tok.type == TT::INT || tok.type == TT::FLOAT)
    {
        res->register_advancement();
        this->advance();

        return res->success(std::make_shared<NumberNode>(tok));
    }

    else if (tok.type == TT::STRING)
    {
        res->register_advancement();
        this->advance();

        return res->success(std::make_shared<StringNode>(tok));
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
        if (res->error->error_name != "") { return res; }

        if (this->current_tok.type == TT::RPAREN)
        {
            res->register_advancement();
            this->advance();

            return res->success(expr);
        }

        else
        {
            return res->failure(std::make_shared<InvalidSyntaxError> (
                this->current_tok.pos_start, this->current_tok.pos_end,
                "Expected ')'"
            ));
        }
    }

    else if (tok.type == TT::LSQUARE)
    {
        ALL_VARIANT list_expr = res->register_result(this->list_expr());
        if (res->error->error_name != "") { return res; }

        return res->success(list_expr);
    }

    else if (tok.matches(TT::KEYWORD, KEYWORDS[4]))
    {
        ALL_VARIANT if_expr = res->register_result(this->if_expr());
        if (res->error->error_name != "") { return res; }

        return res->success(if_expr);
    }

    else if (tok.matches(TT::KEYWORD, KEYWORDS[8]))
    {
        ALL_VARIANT for_expr = res->register_result(this->for_expr());
        if (res->error->error_name != "") { return res; }

        return res->success(for_expr);
    }

    else if (tok.matches(TT::KEYWORD, KEYWORDS[11]))
    {
        ALL_VARIANT while_expr = res->register_result(this->while_expr());
        if (res->error->error_name != "") { return res; }

        return res->success(while_expr);
    }

    else if (tok.matches(TT::KEYWORD, KEYWORDS[12]))
    {
        ALL_VARIANT do_expr = res->register_result(this->do_expr());
        if (res->error->error_name != "") { return res; }

        return res->success(do_expr);
    }

    else if (tok.matches(TT::KEYWORD, KEYWORDS[13]))
    {
        ALL_VARIANT func_def = res->register_result(this->func_def());
        if (res->error->error_name != "") { return res; }

        return res->success(func_def);
    }

    return res->failure(std::make_shared<InvalidSyntaxError>(
        tok.pos_start, tok.pos_end,
        "Expected int, float, identifier, '+', '-', '(', '[', IF', 'FOR', 'WHILE', 'FUN'"
    ));
}

std::shared_ptr<ParseResult> Parser::list_expr()
{
    std::shared_ptr<ParseResult> res = std::make_shared<ParseResult>();

    std::vector<ALL_VARIANT> element_nodes;
    std::shared_ptr<Position> pos_start = this->current_tok.pos_start->copy();

    if (this->current_tok.type != TT::LSQUARE)
    {
        return res->failure(std::make_shared<InvalidSyntaxError> (
            this->current_tok.pos_start, this->current_tok.pos_end,
            "Expected '['"
        ));
    }

    res->register_advancement();
    this->advance();

    if (this->current_tok.type == TT::RSQUARE)
    {
        res->register_advancement();
        this->advance();
    }

    else
    {
        element_nodes.push_back(res->register_result(this->expr()));

        if (res->error->error_name != "")
        {
            return res->failure(std::make_shared<InvalidSyntaxError> (
                this->current_tok.pos_start, this->current_tok.pos_end,
                "Expected ']', 'VAR', 'IF', 'FOR', 'WHILE', 'FUN', int, float, identifier, '+', '-', '(', '[' or 'NOT'"
            ));
        }

        while (this->current_tok.type == TT::COMMA)
        {
            res->register_advancement();
            this->advance();

            element_nodes.push_back(res->register_result(this->expr()));
            if (res->error->error_name != "") { return res; }
        }

        if (this->current_tok.type != TT::RSQUARE)
        {
            return res->failure(std::make_shared<InvalidSyntaxError> (
                this->current_tok.pos_start, this->current_tok.pos_end,
                "Expected ',' or ']'"
            ));
        }

        res->register_advancement();
        this->advance();
    }

    return res->success(std::make_shared<ListNode>(element_nodes, pos_start, this->current_tok.pos_end->copy()));
}

std::shared_ptr<ParseResult> Parser::if_expr()
{
    std::shared_ptr<ParseResult> res = std::make_shared<ParseResult>();
    std::vector<std::pair<ALL_VARIANT, ALL_VARIANT>> cases;
    ALL_VARIANT else_case;

    if (!this->current_tok.matches(TT::KEYWORD, KEYWORDS[4]))
    {
        return res->failure(std::make_shared<InvalidSyntaxError> (
            this->current_tok.pos_start, this->current_tok.pos_end,
            "Expected 'IF'"
        ));
    }

    res->register_advancement();
    this->advance();

    ALL_VARIANT condition = res->register_result(this->expr());
    if (res->error->error_name != "") { return res; }

    if (!this->current_tok.matches(TT::KEYWORD, KEYWORDS[5]))
    {
        return res->failure(std::make_shared<InvalidSyntaxError> (
            this->current_tok.pos_start, this->current_tok.pos_end,
            "Expected 'THEN'"
        ));
    }

    res->register_advancement();
    this->advance();

    ALL_VARIANT expr = res->register_result(this->expr());
    if (res->error->error_name != "") { return res; }

    cases.push_back(std::make_pair(condition, expr));

    while (this->current_tok.matches(TT::KEYWORD, KEYWORDS[6]))
    {
        res->register_advancement();
        this->advance();

        condition = res->register_result(this->expr());
        if (res->error->error_name != "") { return res; }

        if (!this->current_tok.matches(TT::KEYWORD, KEYWORDS[5]))
        {
            return res->failure(std::make_shared<InvalidSyntaxError> (
                this->current_tok.pos_start, this->current_tok.pos_end,
                "Expected 'THEN'"
            ));
        }

        res->register_advancement();
        this->advance();

        expr = res->register_result(this->expr());
        if (res->error->error_name != "") { return res; }

        cases.push_back(std::make_pair(condition, expr));
    }

    if (this->current_tok.matches(TT::KEYWORD, KEYWORDS[7]))
    {
        res->register_advancement();
        this->advance();

        else_case = res->register_result(this->expr());
        if (res->error->error_name != "") { return res; }
    }

    return res->success(std::make_shared<IfNode>(cases, else_case));
}

std::shared_ptr<ParseResult> Parser::for_expr()
{
    std::shared_ptr<ParseResult> res = std::make_shared<ParseResult>();

    if (!this->current_tok.matches(TT::KEYWORD, "FOR"))
    {
        return res->failure(std::make_shared<InvalidSyntaxError> (
            this->current_tok.pos_start, this->current_tok.pos_end,
            "Expected 'FOR'"
        ));
    }

    res->register_advancement();
    this->advance();

    if (this->current_tok.type != TT::IDENTIFIER)
    {
        return res->failure(std::make_shared<InvalidSyntaxError> (
            this->current_tok.pos_start, this->current_tok.pos_end,
            "Expected identifier"
        ));
    }

    Token var_name = this->current_tok;

    res->register_advancement();
    this->advance();

    if (current_tok.type != TT::EQ)
    {
        return res->failure(std::make_shared<InvalidSyntaxError> (
            this->current_tok.pos_start, this->current_tok.pos_end,
            "Expected '='"
        ));
    }

    res->register_advancement();
    this->advance();

    ALL_VARIANT start_value = res->register_result(this->expr());
    if (res->error->error_name != "") { return res; }

    if (!this->current_tok.matches(TT::KEYWORD, KEYWORDS[9]))
    {
        return res->failure(std::make_shared<InvalidSyntaxError> (
            this->current_tok.pos_start, this->current_tok.pos_end,
            "Expected 'UNTIL'"
        ));
    }

    res->register_advancement();
    this->advance();

    ALL_VARIANT end_value = res->register_result(this->expr());
    ALL_VARIANT step_value;

    if (res->error->error_name != "") { return res; }
    if (this->current_tok.matches(TT::KEYWORD, KEYWORDS[10]))
    {
        res->register_advancement();
        this->advance();

        step_value = res->register_result(this->expr());
        if (res->error->error_name != "") { return res; }
    }

    if (!this->current_tok.matches(TT::KEYWORD, KEYWORDS[5]))
    {
        return res->failure(std::make_shared<InvalidSyntaxError> (
            this->current_tok.pos_start, this->current_tok.pos_end,
            "Expected 'THEN'"
        ));
    }

    res->register_advancement();
    this->advance();

    ALL_VARIANT body = res->register_result(this->expr());
    if (res->error->error_name != "") { return res; }

    return res->success(std::make_shared<ForNode>(var_name, start_value, end_value, step_value, body));
}

std::shared_ptr<ParseResult> Parser::while_expr()
{
    std::shared_ptr<ParseResult> res = std::make_shared<ParseResult>();

    if (!this->current_tok.matches(TT::KEYWORD, KEYWORDS[11]))
    {
        return res->failure(std::make_shared<InvalidSyntaxError> (
            this->current_tok.pos_start, this->current_tok.pos_end,
            "Expected 'WHILE'"
        ));
    }

    res->register_advancement();
    this->advance();

    ALL_VARIANT condition = res->register_result(this->expr());
    if (res->error->error_name != "") { return res; }

    if (!this->current_tok.matches(TT::KEYWORD, KEYWORDS[5]))
    {
        return res->failure(std::make_shared<InvalidSyntaxError> (
            this->current_tok.pos_start, this->current_tok.pos_end,
            "Expected 'THEN'"
        ));
    }

    res->register_advancement();
    this->advance();

    ALL_VARIANT body = res->register_result(this->expr());
    if (res->error->error_name != "") { return res; }

    return res->success(std::make_shared<WhileNode>(condition, body));
}

std::shared_ptr<ParseResult> Parser::do_expr()
{
    std::shared_ptr<ParseResult> res = std::make_shared<ParseResult>();

    if (!this->current_tok.matches(TT::KEYWORD, KEYWORDS[12]))
    {
        return res->failure(std::make_shared<InvalidSyntaxError> (
            this->current_tok.pos_start, this->current_tok.pos_end,
            "Expected 'DO'"
        ));
    }

    /* Go to body of statment */
    res->register_advancement();
    this->advance();

    /* Body */
    ALL_VARIANT body = res->register_result(this->expr());
    if (res->error->error_name != "") { return res; }

    /* We do not need to advance since we last call was `expr()` function */

    if (!this->current_tok.matches(TT::KEYWORD, KEYWORDS[11]))
    {
        return res->failure(std::make_shared<InvalidSyntaxError> (
            this->current_tok.pos_start, this->current_tok.pos_end,
            "Expected 'WHILE'"
        ));
    }

    /* Go to condition of statment */
    res->register_advancement();
    this->advance();

    ALL_VARIANT condition = res->register_result(this->expr());
    if (res->error->error_name != "") { return res; }

    return res->success(std::make_shared<DoNode>(body, condition));
}

std::shared_ptr<ParseResult> Parser::func_def()
{
    std::shared_ptr<ParseResult> res = std::make_shared<ParseResult>();

    if (!this->current_tok.matches(TT::KEYWORD, KEYWORDS[13]))
    {
        return res->failure(std::make_shared<InvalidSyntaxError> (
            this->current_tok.pos_start, this->current_tok.pos_end,
            "Expected 'FN'"
        ));
    }

    res->register_advancement();
    this->advance();

    Token var_name_tok = Token(TT::NUL);
    if (this->current_tok.type == TT::IDENTIFIER)
    {
        var_name_tok = this->current_tok;

        res->register_advancement();
        this->advance();

        if (this->current_tok.type != TT::LPAREN)
        {
            return res->failure(std::make_shared<InvalidSyntaxError> (
                this->current_tok.pos_start, this->current_tok.pos_end,
                "Expected '('"
            ));
        }
    }

    else
    {
        if (this->current_tok.type != TT::LPAREN)
        {
            return res->failure(std::make_shared<InvalidSyntaxError> (
                this->current_tok.pos_start, this->current_tok.pos_end,
                "Expected identifier or '('"
            ));
        }
    }

    res->register_advancement();
    this->advance();

    std::vector<Token> arg_name_toks;

    if (this->current_tok.type == TT::IDENTIFIER)
    {
        arg_name_toks.push_back(this->current_tok);
        
        res->register_advancement();
        this->advance();

        while (this->current_tok.type == TT::COMMA)
        {
            res->register_advancement();
            this->advance();

            if (this->current_tok.type != TT::IDENTIFIER)
            {
                return res->failure(std::make_shared<InvalidSyntaxError> (
                    this->current_tok.pos_start, this->current_tok.pos_end,
                    "Expected identifier"
                ));
            }

            arg_name_toks.push_back(this->current_tok);

            res->register_advancement();
            this->advance();
        }

        if (this->current_tok.type != TT::RPAREN)
        {
            return res->failure(std::make_shared<InvalidSyntaxError> (
                this->current_tok.pos_start, this->current_tok.pos_end,
                "Expected ',' or ')'"
            ));
        }
    }

    else
    {
        if (this->current_tok.type != TT::RPAREN)
        {
            return res->failure(std::make_shared<InvalidSyntaxError> (
                this->current_tok.pos_start, this->current_tok.pos_end,
                "Expected identifier or ')'"
            ));
        }
    }

    res->register_advancement();
    this->advance();

    if (this->current_tok.type != TT::ARROW)
    {
        return res->failure(std::make_shared<InvalidSyntaxError> (
            this->current_tok.pos_start, this->current_tok.pos_end,
            "Expected '->'"
        ));
    }

    res->register_advancement();
    this->advance();

    ALL_VARIANT node_to_return = res->register_result(this->expr());
    if (res->error->error_name != "") { return res; }

    return res->success(std::make_shared<FuncDefNode>(var_name_tok, arg_name_toks, node_to_return));
}

std::shared_ptr<ParseResult> Parser::bin_op(std::function<std::shared_ptr<ParseResult>()> func_a, std::vector<std::pair<TT, std::string>>& ops, std::function<std::shared_ptr<ParseResult>()> func_b)
{
    if (func_b == nullptr)
    {
        func_b = func_a;
    }

    std::shared_ptr<ParseResult> res = std::make_shared<ParseResult>();
    auto left = res->register_result(func_a());

    if (res->error->error_name != "") { return res; }
    while (this->contains(ops, this->current_tok.type, this->current_tok.value))
    {
        Token op_tok = this->current_tok;
        
        res->register_advancement();
        this->advance();

        auto right = res->register_result(func_b());
        if (res->error->error_name != "") { return res; }

        left = std::make_shared<BinOpNode> (
            left,
            op_tok,
            right
        );
    }

    return res->success(left);
}

bool Parser::contains(std::vector<std::pair<TT, std::string>>& ops, TT& type, std::string& value)
{
    for (auto &&i : ops)
    {
        if (i.first == type && i.second == value) { return true; }
    }

    return false;
}
