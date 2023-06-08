/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <variant>

#include "Context.hpp"
#include "Error.hpp"
#include "Nodes.hpp"
#include "Position.hpp"
#include "Token.hpp"

#include "../empty.hpp"

class Number : public std::enable_shared_from_this<Number>
{
    public:
        Number(signed long value, std::shared_ptr<Context> context=nullptr, Position pos_start = EMPTY_POSITION, Position pos_end = EMPTY_POSITION);
        ~Number();

        std::string repr();

        std::tuple<std::shared_ptr<Number>, Error> added_to(std::variant<std::shared_ptr<Number>, ParseResult> other);
        std::tuple<std::shared_ptr<Number>, Error> subbed_by(std::variant<std::shared_ptr<Number>, ParseResult> other);
        std::tuple<std::shared_ptr<Number>, Error> multed_by(std::variant<std::shared_ptr<Number>, ParseResult> other);
        std::tuple<std::shared_ptr<Number>, Error> dived_by(std::variant<std::shared_ptr<Number>, ParseResult> other);

    public:
        Error error;
        signed long value;

        Position pos_start;
        Position pos_end;

        std::shared_ptr<Context> context;
};

static const std::shared_ptr<Number> EMPTY_NUMBER;
