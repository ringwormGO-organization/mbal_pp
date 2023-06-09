/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "Token.hpp"
#include "Nodes.hpp"

#include "define.hpp"

#define PARSE_REGISTER_TYPES std::variant<Token, std::shared_ptr<ParseResult>, ALL_VARIANT>

static Token current_tok = Token(TT::NUL);

static const Token EMPTY_TOKEN_2 = Token(TT::NUL);

class ParseResult : public std::enable_shared_from_this<ParseResult>
{
    public:
        ParseResult();
        ~ParseResult();

        PARSE_REGISTER_TYPES register_result(PARSE_REGISTER_TYPES res);

        std::shared_ptr<ParseResult> success(ALL_VARIANT node);
        std::shared_ptr<ParseResult> failure(std::shared_ptr<Error> error);

    public:
        std::shared_ptr<Error> error = std::make_shared<Error>(EMPTY_POSITION, EMPTY_POSITION, "", "");
        ALL_VARIANT node;
};

class Parser
{
    public:
        Parser(std::vector<Token> tokens);
        virtual ~Parser();

        Token advance();
        std::shared_ptr<ParseResult> parse();

        std::shared_ptr<ParseResult> atom();
        std::shared_ptr<ParseResult> power();
        std::shared_ptr<ParseResult> factor();
        std::shared_ptr<ParseResult> term();
        std::shared_ptr<ParseResult> expr();

        template <int N>
        std::shared_ptr<ParseResult> bin_op(std::function<std::shared_ptr<ParseResult>()> func_a, TT ops[N], std::function<std::shared_ptr<ParseResult>()> func_b = nullptr);

        std::vector<Token> tokens;
    private:
        signed long tok_idx = -1;

        template <int N>
        bool contains(TT current, TT ops[N]);
};
