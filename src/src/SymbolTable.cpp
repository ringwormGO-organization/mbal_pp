/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "SymbolTable.hpp"

SymbolTable::SymbolTable(std::shared_ptr<Context> parent)
{
    if (parent != nullptr)
    {
        this->parent = parent;
    }
}

SymbolTable::~SymbolTable()
{
    
}

std::variant<std::shared_ptr<Value>, std::nullptr_t> SymbolTable::get_value(std::string name)
{
    std::variant<std::shared_ptr<Value>, std::nullptr_t> value;

    std::map<std::string, std::variant<std::shared_ptr<Value>, std::nullptr_t>>::const_iterator pos = this->symbols.find(name);
    
    if (pos == symbols.end() && this->parent->display_name != "")
    {
        /* TODO */
    }

    else if (pos == symbols.end() && this->parent->display_name != "")
    {
        return nullptr;
    }

    value = pos->second;
    return value;
}

void SymbolTable::set(std::string name, std::variant<std::shared_ptr<Value>, std::nullptr_t> value)
{
    this->remove(name);
    this->symbols.insert({ name, value });
}

void SymbolTable::remove(std::string name)
{
    this->symbols.erase(name);
}
