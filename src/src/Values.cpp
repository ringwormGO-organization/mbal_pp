/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Values.hpp"

#include "Context.hpp"
#include "Error.hpp"
#include "Position.hpp"
#include "Token.hpp"

Number::Number(signed long value, std::shared_ptr<Context> context, std::shared_ptr<Position> pos_start, std::shared_ptr<Position> pos_end)
{
    this->value = value;

    this->pos_start = pos_start;
    this->pos_end = pos_end;

    this->context = context;
}

Number::~Number()
{

}

std::string Number::repr()
{
    return std::to_string(this->value);
}

std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> Number::added_to(std::variant<std::shared_ptr<Number>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Number>>(other))
    {
        return { std::make_shared<Number>(this->value + std::get<0>(other).get()->value, this->context), std::make_shared<NoError>() };
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> Number::subbed_by(std::variant<std::shared_ptr<Number>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Number>>(other))
    {
        return { std::make_shared<Number>(this->value - std::get<0>(other).get()->value, this->context), std::make_shared<NoError>() };
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> Number::multed_by(std::variant<std::shared_ptr<Number>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Number>>(other))
    {
        return { std::make_shared<Number>(this->value * std::get<0>(other).get()->value, this->context), std::make_shared<NoError>() };
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> Number::dived_by(std::variant<std::shared_ptr<Number>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Number>>(other))
    {
        if (std::get<0>(other)->value == 0)
        {
            return { EMPTY_NUMBER, std::make_shared<RTError> (
                std::get<0>(other)->pos_start, std::get<0>(other)->pos_start,
                "Division by zero",
                this->context
            ) };
        }

        return { std::make_shared<Number>(this->value / std::get<0>(other).get()->value, this->context), std::make_shared<NoError>() };
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::tuple<std::shared_ptr<Number>, std::shared_ptr<Error>> Number::powed_by(std::variant<std::shared_ptr<Number>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Number>>(other))
    {
        return { std::make_shared<Number>(pow(this->value, std::get<0>(other).get()->value), this->context), std::make_shared<NoError>() };
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}
