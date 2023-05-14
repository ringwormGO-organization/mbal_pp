/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Token.hpp"

bool operator==(const Position& pos1, const Position& pos2) 
{
    bool result = false;

    if (pos1.col == pos2.col) result = true;
    else result = false;

    if (pos1.fn == pos2.fn) result = true;
    else result = false;

    if (pos1.ftxt == pos2.ftxt) result = true;
    else result = false;

    if (pos1.idx == pos2.idx) result = true;
    else result = false;

    if (pos1.ln == pos2.ln) result = true;
    else result = false;

    return result;
}

bool operator!=(const Position& pos1, const Position& pos2) 
{
    return !(pos1 == pos2);
}

Token::Token(TT type, Position pos_start, std::string value, Position pos_end) : pos_start(pos_start), pos_end(pos_end)
{
    this->type = type;
    this->value = value;

    if (pos_start != EMPTY_POSITION)
    {
        this->pos_start = pos_start.copy();
        this->pos_end = pos_start.copy();
        
        this->pos_end.advance();
    }

    if (pos_end != EMPTY_POSITION)
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
