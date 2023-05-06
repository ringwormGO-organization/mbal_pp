/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Position.hpp"

Position::Position(signed long idx, signed long ln, signed long col, std::string fn, std::string ftxt)
{
    this->idx = idx;
    this->ln = ln;
    this->col = col;

    this->fn = fn;
    this->ftxt = ftxt;
}

Position::~Position()
{

}

Position* Position::advance(char current_char)
{
    this->idx += 1;
    this->col += 1;

    if (current_char == '\n')
    {
        this->ln += 1;
        this->col = 0;
    }

    return this;
}

Position Position::copy()
{
    return Position(this->idx, this->ln, this->col, this->fn, this->ftxt);
}
