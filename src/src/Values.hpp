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
        Number(double value, std::shared_ptr<Context> context=nullptr, std::shared_ptr<Position> pos_start = nullptr, std::shared_ptr<Position> pos_end = nullptr);
        virtual ~Number();

        /**
         * Returns value as string
         * @return std::string
        */
        std::string repr();

        std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> added_to(std::variant<std::shared_ptr<Number>, std::nullptr_t> other);
        std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> subbed_by(std::variant<std::shared_ptr<Number>, std::nullptr_t> other);
        std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> multed_by(std::variant<std::shared_ptr<Number>, std::nullptr_t> other);
        std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> dived_by(std::variant<std::shared_ptr<Number>, std::nullptr_t> other);
        std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> powed_by(std::variant<std::shared_ptr<Number>, std::nullptr_t> other);

        std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> get_comparison_eq(std::variant<std::shared_ptr<Number>, std::nullptr_t> other);
        std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> get_comparison_ne(std::variant<std::shared_ptr<Number>, std::nullptr_t> other);
        std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> get_comparison_lt(std::variant<std::shared_ptr<Number>, std::nullptr_t> other);
        std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> get_comparison_gt(std::variant<std::shared_ptr<Number>, std::nullptr_t> other);
        std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> get_comparison_lte(std::variant<std::shared_ptr<Number>, std::nullptr_t> other);
        std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> get_comparison_gte(std::variant<std::shared_ptr<Number>, std::nullptr_t> other);

        std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> anded_by(std::variant<std::shared_ptr<Number>, std::nullptr_t> other);
        std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> ored_by(std::variant<std::shared_ptr<Number>, std::nullptr_t> other);
        std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> notted_by();

        std::shared_ptr<Number> copy();
        bool is_true();
    public:
        std::shared_ptr<Error> error;
        double value = 0.0;

        std::shared_ptr<Position> pos_start;
        std::shared_ptr<Position> pos_end;

        std::shared_ptr<Context> context;
};

static const std::shared_ptr<Number> EMPTY_NUMBER;
