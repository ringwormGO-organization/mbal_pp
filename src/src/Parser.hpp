/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "Error.hpp"
#include "Token.hpp"
#include "Nodes.hpp"

#include "define.hpp"

#define PARSE_REGISTER_TYPES std::variant<Token, std::shared_ptr<ParseResult>, ALL_VARIANT>

class ParseResult : public std::enable_shared_from_this<ParseResult>
{
    public:
        ParseResult();
        ~ParseResult();

        void register_advancement();
        ALL_VARIANT register_result(std::shared_ptr<ParseResult> res);

        std::shared_ptr<ParseResult> success(ALL_VARIANT node);
        std::shared_ptr<ParseResult> failure(std::shared_ptr<Error> error);

    public:
        size_t advance_count = 0;

        std::shared_ptr<Error> error = std::make_shared<Error>(std::make_shared<Position>(0, 0, 0, "", ""), std::make_shared<Position>(0, 0, 0, "", ""), "", "");
        ALL_VARIANT node;
};

/* ---------------------------------------------------------------------------- */

class Parser
{
    public:
        Parser(std::vector<Token> tokens);
        virtual ~Parser();

        Token advance();
        std::shared_ptr<ParseResult> parse();

        std::shared_ptr<ParseResult> if_expr();
        std::shared_ptr<ParseResult> for_expr();
        std::shared_ptr<ParseResult> while_expr();
        std::shared_ptr<ParseResult> func_def();
        std::shared_ptr<ParseResult> do_expr();
        std::shared_ptr<ParseResult> call();
        std::shared_ptr<ParseResult> atom();
        std::shared_ptr<ParseResult> power();
        std::shared_ptr<ParseResult> factor();
        std::shared_ptr<ParseResult> term();
        std::shared_ptr<ParseResult> arith_expr();
        std::shared_ptr<ParseResult> comp_expr();
        std::shared_ptr<ParseResult> expr();

        std::shared_ptr<ParseResult> bin_op(std::function<std::shared_ptr<ParseResult>()> func_a, std::vector<std::pair<TT, std::string>>& ops, std::function<std::shared_ptr<ParseResult>()> func_b = nullptr);

        std::vector<Token> tokens;
    private:
        Token current_tok;

        size_t tok_idx = 0;
        bool first_run = true;

        bool contains(std::vector<std::pair<TT, std::string>>& ops, TT& type, std::string& value);
};
