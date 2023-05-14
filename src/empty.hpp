/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <variant>
#include <vector>

#include "src/Token.hpp"
#include "src/Parser.hpp"

static const Token EMPTY_TOKEN = Token(TT::NUL);
static const std::vector<Token> EMPTY_VECTOR;

static const std::variant<NumberNode, BinOpNode> EMPTY_NODE = NumberNode(EMPTY_TOKEN);
static const Parser EMPTY_PARSER = Parser(EMPTY_VECTOR);

static const ParseResult EMPTY_PARSE_RESULT = ParseResult();
