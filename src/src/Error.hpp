/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <algorithm>
#include <exception>
#include <iostream>
#include <string>

#include "Position.hpp"

static const Position EMPTY_POSITION = Position(-1, 0, -1, "", "");

std::string string_with_arrows(std::string text, Position pos_start, Position pos_end);

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
};

class InvalidSyntaxError : public Error
{
    public:
        InvalidSyntaxError(Position pos_start, Position pos_end, std::string details) : Error(pos_start, pos_end, "Invalid Syntax", details) {};
};

class RTError : public Error
{
    public:
        RTError(Position pos_start, Position pos_end, std::string details) : Error(pos_start, pos_end, "Runtime Error", details) {};
};

class NoError : public Error
{
    public:
        NoError() : Error(EMPTY_POSITION, EMPTY_POSITION, "", "") {};
};

/* ---------------------------------------------------------------------------- */

class InterpreterWrongType : public std::exception
{
    public:
        InterpreterWrongType() noexcept = default;

        const char* what() const noexcept override { return "[InterpreterWrongType] Wrong type in `node` variable"; }
};
