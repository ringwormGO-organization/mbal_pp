/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <variant>

#include "Nodes.hpp"
#include "Position.hpp"
#include "Token.hpp"

#include "../empty.hpp"

class Number : public std::enable_shared_from_this<Number>
{
    public:
        Number(signed long value);
        ~Number();

        std::string repr();
        std::shared_ptr<Number> set_pos(Position pos_start=EMPTY_POSITION, Position pos_end=EMPTY_POSITION);

        std::shared_ptr<Number> added_to(std::variant<std::shared_ptr<Number>, ParseResult> other);
        std::shared_ptr<Number> subbed_by(std::variant<std::shared_ptr<Number>, ParseResult> other);
        std::shared_ptr<Number> multed_by(std::variant<std::shared_ptr<Number>, ParseResult> other);
        std::shared_ptr<Number> dived_by(std::variant<std::shared_ptr<Number>, ParseResult> other);

    private:
        signed long value;

        Position pos_start;
        Position pos_end;
};
