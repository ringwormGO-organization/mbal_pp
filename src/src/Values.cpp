/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Values.hpp"

Number::Number(signed long value) : pos_start(pos_start), pos_end(pos_end)
{
    this->value = value;
    this->set_pos();
}

Number::~Number()
{

}

std::string Number::repr()
{
    return std::to_string(this->value);
}

std::shared_ptr<Number> Number::set_pos(Position pos_start, Position pos_end)
{
    this->pos_start = pos_start;
    this->pos_end = pos_end;

    return std::shared_ptr<Number>(shared_from_this());
}

std::shared_ptr<Number> Number::added_to(std::variant<std::shared_ptr<Number>, ParseResult> other)
{
    if (std::holds_alternative<std::shared_ptr<Number>>(other))
    {
        return std::make_shared<Number>(Number(this->value + std::get<0>(other).get()->value));
    }
}

std::shared_ptr<Number> Number::subbed_by(std::variant<std::shared_ptr<Number>, ParseResult> other)
{
    if (std::holds_alternative<std::shared_ptr<Number>>(other))
    {
        return std::make_shared<Number>(Number(this->value - std::get<0>(other).get()->value));
    }
}

std::shared_ptr<Number> Number::multed_by(std::variant<std::shared_ptr<Number>, ParseResult> other)
{
    if (std::holds_alternative<std::shared_ptr<Number>>(other))
    {
        return std::make_shared<Number>(Number(this->value * std::get<0>(other).get()->value));
    }
}

std::shared_ptr<Number> Number::dived_by(std::variant<std::shared_ptr<Number>, ParseResult> other)
{
    if (std::holds_alternative<std::shared_ptr<Number>>(other))
    {
        return std::make_shared<Number>(Number(this->value / std::get<0>(other).get()->value));
    }
}
