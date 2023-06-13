/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Token.hpp"

Token::Token(TT type, std::shared_ptr<Position> pos_start, std::string value, std::shared_ptr<Position> pos_end)
{
    this->type = type;
    this->value = value;

    if (pos_start != nullptr && pos_end != nullptr)
    {
        this->pos_start = pos_start->copy();
        this->pos_end = pos_start->copy();

        this->pos_end->advance();
        this->pos_end = pos_end;
    }
}

Token::~Token()
{

}

bool Token::matches(TT type, std::string value)
{
    return (this->type == type && this->value == value);
}

std::string Token::repr()
{
    if (this->value.size() > 0)
    {
        return (enum_to_string() += ": ") += (this->value += ", ");
    }

    return (enum_to_string() += ", ");
}

std::string Token::enum_to_string()
{
    switch (this->type)
    {
        case TT::INT: return "INT";
        case TT::FLOAT: return "FLOAT";
        case TT::PLUS: return "PLUS";
        case TT::MINUS: return "MINUS";
        case TT::MUL: return "MUL";
        case TT::DIV: return "DIV";
        case TT::LPAREN: return "LPAREN";
        case TT::RPAREN: return "RPAREN";
        default: return "n/a";
    }
}
