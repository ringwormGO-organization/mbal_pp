/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Token.hpp"

Token::Token(TT type, std::string value)
{
    this->type = type;
    this->value = value;
}

Token::~Token()
{

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
