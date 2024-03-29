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
        STRING,

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
        LSQUARE,
        RSQUARE,

    /* Identifiers */
        IDENTIFIER,
        KEYWORD,

    /* Comparison operators */
        EE,
        NE,
        LT,
        GT,
        LTE,
        GTE,

    /* Functions */
        COMMA,
        ARROW,
        NEWLINE,

    /* Other */
        END_OF_FILE,
        NUL,
};

static const std::string KEYWORDS[] = {
    /* Declaring variables */
        "LET",
    
    /* Logical operators */
        "AND",
        "OR",
        "NOT",

    /* IF statments */
        "IF",
        "THEN",
        "ELSE IF",
        "ELSE",

    /* FOR, WHILE and DO statment */
        "FOR",
        "UNTIL",
        "STEP",
        "WHILE",
        "DO",

    /* Functions */
        "FN"
};

class Token
{
    public:
        Token(TT type, std::shared_ptr<Position> pos_start = std::make_shared<Position>(0, 0, 0, "", ""), std::string value = "", std::shared_ptr<Position> pos_end = std::make_shared<Position>(0, 0, 0, "", ""));
        ~Token();

        bool matches(TT type, std::string value);
        std::string repr();
        std::string enum_to_string();

    public:
        TT type;
        std::string value = "";

        std::shared_ptr<Position> pos_start;
        std::shared_ptr<Position> pos_end;
};
