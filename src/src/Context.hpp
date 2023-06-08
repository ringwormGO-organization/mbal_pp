/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "Position.hpp"

class Context
{
    public:
        Context(std::string display_name, std::shared_ptr<Context> parent=nullptr, Position parent_entry_pos=Position(-1, 0, -1, "", ""));
        ~Context();

    public:
        std::string display_name;
        std::shared_ptr<Context> parent;
        Position parent_entry_pos;
};
