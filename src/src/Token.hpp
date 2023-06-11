/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <string>

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
        Token(TT type, Position pos_start = Position(-1, 0, -1, "", ""), std::string value = "", Position pos_end = Position(-1, 0, -1, "", ""));
        ~Token();

        std::string repr();
        std::string enum_to_string();

    public:
        TT type;
        std::string value = "";

        Position pos_start;
        Position pos_end;
};
