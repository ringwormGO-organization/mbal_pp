/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Values.hpp"

Number::Number(signed long value, std::shared_ptr<Context> context, Position pos_start, Position pos_end) : error(EMPTY_POSITION, EMPTY_POSITION, "", ""), pos_start(pos_start), pos_end(pos_end)
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

std::tuple<std::shared_ptr<Number>, Error> Number::added_to(std::variant<std::shared_ptr<Number>, ParseResult> other)
{
    if (std::holds_alternative<std::shared_ptr<Number>>(other))
    {
        return { std::make_shared<Number>(this->value + std::get<0>(other).get()->value), NoError() };
    }
}

std::tuple<std::shared_ptr<Number>, Error> Number::subbed_by(std::variant<std::shared_ptr<Number>, ParseResult> other)
{
    if (std::holds_alternative<std::shared_ptr<Number>>(other))
    {
        return { std::make_shared<Number>(this->value - std::get<0>(other).get()->value, this->context), NoError() };
    }
}

std::tuple<std::shared_ptr<Number>, Error> Number::multed_by(std::variant<std::shared_ptr<Number>, ParseResult> other)
{
    if (std::holds_alternative<std::shared_ptr<Number>>(other))
    {
        return { std::make_shared<Number>(this->value * std::get<0>(other).get()->value, this->context), NoError() };
    }
}

std::tuple<std::shared_ptr<Number>, Error> Number::dived_by(std::variant<std::shared_ptr<Number>, ParseResult> other)
{
    if (std::holds_alternative<std::shared_ptr<Number>>(other))
    {
        if (std::get<0>(other)->value == 0)
        {
            return { EMPTY_NUMBER, RTError(
                std::get<0>(other)->pos_start, std::get<0>(other)->pos_start,
                "Division by zero",
                this->context
            ) };
        }

        return { std::make_shared<Number>(this->value / std::get<0>(other).get()->value, this->context), NoError() };
    }
}
