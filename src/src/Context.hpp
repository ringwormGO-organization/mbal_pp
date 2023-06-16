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

/**
 * Context class
 * @param display_name display name
 * @param parent parent
 * @param parent_entry_pos parent entry position
*/
class Context
{
    public:
        Context(std::string display_name, std::shared_ptr<Context> parent=nullptr, std::shared_ptr<Position> parent_entry_pos=nullptr);
        virtual ~Context();

    public:
        std::string display_name;
        std::shared_ptr<Context> parent;
        std::shared_ptr<Position> parent_entry_pos;
        std::shared_ptr<SymbolTable> symbol_table;          /* table containing symbols (variables) */
};
