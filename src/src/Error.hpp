/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <string>

class Error
{
    public:
        Error(std::string error_name, std::string details);
        ~Error();

        std::string as_string();
        std::string error_name = "";
    private:
        std::string details = "";
};

class IllegalCharError : public Error
{
    public:
        IllegalCharError(std::string details) : Error("Illegal Character", details) {};
        ~IllegalCharError() {};
};

class NoError : public Error
{
    public:
        NoError() : Error("", "") {};
        ~NoError() {};
};
