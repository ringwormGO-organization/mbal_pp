/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <memory>
#include <string>

class Position : public std::enable_shared_from_this<Position>
{
    public:
        Position(size_t idx, size_t ln, size_t col, std::string fn, std::string ftxt);
        ~Position();

        std::shared_ptr<Position> advance(char current_char = '\0');
        std::shared_ptr<Position> copy();
    public:
        size_t idx = 0;
        size_t ln = 0;
        size_t col = 0;

        std::string fn = "";
        std::string ftxt = "";
};
