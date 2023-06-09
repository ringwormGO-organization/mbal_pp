/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <string>

#include "Error.hpp"
#include "Position.hpp"

enum TT {
    INT,
    FLOAT,
    PLUS,
    MINUS,
    MUL,
    DIV,
    POW,
    LPAREN,
    RPAREN,
    END_OF_FILE,
    NUL,
};

class Token
{
    public:
        Token(TT type, Position pos_start = EMPTY_POSITION, std::string value = "", Position pos_end = EMPTY_POSITION);
        ~Token();

        std::string repr();
        std::string enum_to_string();

    public:
        TT type;
        std::string value = "";

        Position pos_start;
        Position pos_end;
};
