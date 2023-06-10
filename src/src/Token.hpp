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

enum TT 
{
    /* Variable type */
        INT,
        FLOAT,

    /* Operators */
        PLUS,
        MINUS,
        MUL,
        DIV,
        POW,
        EQ,

    /* Parentheses */
        LPAREN,
        RPAREN,

    /* Identifiers */
        IDENTIFIER,
        KEYWORD,

    /* Other */
        END_OF_FILE,
        NUL,
};

static const std::string KEYWORDS[] = {
    "VAR",
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
