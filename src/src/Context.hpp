/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <memory>
#include <string>

class Position;
class SymbolTable;

class Context
{
    public:
        Context(std::string display_name, std::shared_ptr<Context> parent=nullptr, std::shared_ptr<Position> parent_entry_pos=nullptr);
        virtual ~Context();

    public:
        std::string display_name;
        std::shared_ptr<Context> parent;
        std::shared_ptr<Position> parent_entry_pos;
        std::shared_ptr<SymbolTable> symbol_table;
};
