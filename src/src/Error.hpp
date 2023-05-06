/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <string>

#include "Position.hpp"

static const Position EMPTY_POSITION = Position(-1, 0, -1, "", "");

class Error
{
    public:
        Error(Position pos_start, Position pos_end, std::string error_name, std::string details);
        virtual ~Error() {};

        std::string as_string();
        std::string error_name = "";
    protected:
        Position pos_start;
        Position pos_end;
        std::string details = "";
};

class IllegalCharError : public Error
{
    public:
        IllegalCharError(Position pos_start, Position pos_end, std::string details) : Error(pos_start, pos_end, "Illegal Character", details) {};
        ~IllegalCharError() {};
};

class NoError : public Error
{
    public:
        NoError() : Error(EMPTY_POSITION, EMPTY_POSITION, "", "") {};
        ~NoError() {};
};
