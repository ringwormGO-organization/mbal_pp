/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <string>

class Position
{
    public:
        Position(signed long idx, signed long ln, signed long col, std::string fn, std::string ftxt);
        ~Position();

        Position* advance(char current_char = '\0');
        Position copy();
    public:
        signed long idx = -1;
        signed long ln = -1;
        signed long col = -1;

        std::string fn = "";
        std::string ftxt = "";
};
