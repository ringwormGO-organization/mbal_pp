/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include "Token.hpp"
#include "Nodes.hpp"

#define PARSE_REGISTER_TYPES std::variant<Token, ParseResult, std::variant<NumberNode*, BinOpNode*, UnaryOpNode*>>

static Token current_tok = Token(TT::NUL);

static const Token EMPTY_TOKEN_2 = Token(TT::NUL);

class ParseResult
{
    public:
        ParseResult() : error(EMPTY_POSITION, EMPTY_POSITION, "", ""), node(std::move(node)) {};
        ~ParseResult() {};

        PARSE_REGISTER_TYPES register_result(PARSE_REGISTER_TYPES res);

        ParseResult& success(std::variant<NumberNode*, BinOpNode*, UnaryOpNode*> node);
        ParseResult& failure(Error error);

    public:
        Error error;
        std::variant<NumberNode*, BinOpNode*, UnaryOpNode*> node;
};

class Parser
{
    public:
        Parser(std::vector<Token> tokens);
        virtual ~Parser();

        Token advance();
        ParseResult parse();

        ParseResult factor();
        ParseResult term();
        ParseResult expr();

        template <int N>
        ParseResult bin_op(std::function<ParseResult()> func, TT ops[N]);

        std::vector<Token> tokens;
    private:
        signed long tok_idx = -1;

        template <int N>
        bool contains(TT current, TT ops[N]);
};

std::variant<std::monostate, NumberNode*, BinOpNode*> GetSubset(const std::variant<NumberNode*, BinOpNode*, UnaryOpNode*>& vin);
std::variant<std::monostate, NumberNode*, BinOpNode*> GetSubset2(const std::variant<NumberNode*, BinOpNode*, UnaryOpNode*>& vin);
