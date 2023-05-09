/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <string>

enum TT {
    INT,
    FLOAT,
    PLUS,
    MINUS,
    MUL,
    DIV,
    LPAREN,
    RPAREN,
    NUL,
};

class Token
{
    public:
        Token(TT type, std::string value = "");
        ~Token();

        std::string repr();
        std::string enum_to_string();

        TT type;
    private:
        std::string value = "";

};
