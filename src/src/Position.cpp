/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Position.hpp"

Position::Position(size_t idx, size_t ln, size_t col, std::string fn, std::string ftxt)
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

std::shared_ptr<Position> Position::advance(char current_char)
{
    this->idx += 1;
    this->col += 1;

    if (current_char == '\n')
    {
        this->ln += 1;
        this->col = 0;
    }

    return std::shared_ptr<Position>(shared_from_this());
}

std::shared_ptr<Position> Position::copy()
{
    return std::make_shared<Position>(this->idx, this->ln, this->col, this->fn, this->ftxt);
}
