/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Token.hpp"

Token::Token(TT type, Position pos_start, std::string value, Position pos_end) : pos_start(pos_start), pos_end(pos_end)
{
    this->type = type;
    this->value = value;

    if (pos_start.idx != -1 && pos_start.col != -1)
    {
        this->pos_start = pos_start.copy();
        this->pos_end = pos_start.copy();
        
        this->pos_end.advance();
    }

    if (pos_end.idx != -1 && pos_end.col != -1)
    {
        this->pos_end = pos_end;
    }
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
