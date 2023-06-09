/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Context.hpp"

#include "Position.hpp"
#include "SymbolTable.hpp"

Context::Context(std::string display_name, std::shared_ptr<Context> parent, std::shared_ptr<Position> parent_entry_pos)
{
    this->display_name = display_name;
    this->parent = parent;
    this->parent_entry_pos = parent_entry_pos;
}

Context::~Context()
{

}
