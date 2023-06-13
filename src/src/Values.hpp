/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <math.h>
#include <memory>
#include <string>
#include <tuple>
#include <variant>

class Context;
class Error;
class Position;

class Number : public std::enable_shared_from_this<Number>
{
    public:
        Number(signed long value, std::shared_ptr<Context> context=nullptr, std::shared_ptr<Position> pos_start = nullptr, std::shared_ptr<Position> pos_end = nullptr);
        virtual ~Number();

        std::string repr();

        std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> added_to(std::variant<std::shared_ptr<Number>, std::nullptr_t> other);
        std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> subbed_by(std::variant<std::shared_ptr<Number>, std::nullptr_t> other);
        std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> multed_by(std::variant<std::shared_ptr<Number>, std::nullptr_t> other);
        std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> dived_by(std::variant<std::shared_ptr<Number>, std::nullptr_t> other);
        std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> powed_by(std::variant<std::shared_ptr<Number>, std::nullptr_t> other);

    public:
        std::shared_ptr<Error> error;
        signed long value;

        std::shared_ptr<Position> pos_start;
        std::shared_ptr<Position> pos_end;

        std::shared_ptr<Context> context;
};

static const std::shared_ptr<Number> EMPTY_NUMBER;
