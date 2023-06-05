/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Values.hpp"

Number::Number(signed long value, Position pos_start, Position pos_end) : value(value), pos_start(pos_start), pos_end(pos_end)
{
    this->value = value;

    this->pos_start = pos_start;
    this->pos_end = pos_end;
}

Number::~Number()
{

}

std::string Number::repr()
{
    return std::to_string(this->value);
}

std::shared_ptr<Number> Number::added_to(std::variant<std::shared_ptr<Number>, ParseResult> other)
{
    if (std::holds_alternative<std::shared_ptr<Number>>(other))
    {
        return std::make_shared<Number>(this->value + std::get<0>(other).get()->value);
    }
}

std::shared_ptr<Number> Number::subbed_by(std::variant<std::shared_ptr<Number>, ParseResult> other)
{
    if (std::holds_alternative<std::shared_ptr<Number>>(other))
    {
        return std::make_shared<Number>(this->value - std::get<0>(other).get()->value);
    }
}

std::shared_ptr<Number> Number::multed_by(std::variant<std::shared_ptr<Number>, ParseResult> other)
{
    if (std::holds_alternative<std::shared_ptr<Number>>(other))
    {
        return std::make_shared<Number>(this->value * std::get<0>(other).get()->value);
    }
}

std::shared_ptr<Number> Number::dived_by(std::variant<std::shared_ptr<Number>, ParseResult> other)
{
    if (std::holds_alternative<std::shared_ptr<Number>>(other))
    {
        return std::make_shared<Number>(this->value / std::get<0>(other).get()->value);
    }
}
