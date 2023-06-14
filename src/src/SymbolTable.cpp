/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "SymbolTable.hpp"

std::variant<std::shared_ptr<Number>, std::nullptr_t> SymbolTable::get_value(std::string name)
{
    std::variant<std::shared_ptr<Number>, std::nullptr_t> value;

    std::map<std::string, std::variant<std::shared_ptr<Number>, std::nullptr_t>>::const_iterator pos = this->symbols.find(name);
    
    if (pos == symbols.end() && this->parent.type() != typeid(std::nullptr_t))
    {
        /* TODO */
    }

    else if (pos == symbols.end() && this->parent.type() == typeid(std::nullptr_t))
    {
        return nullptr;
    }

    value = pos->second;
    return value;
}

void SymbolTable::set(std::string name, std::variant<std::shared_ptr<Number>, std::nullptr_t> value)
{
    this->symbols.insert({ name, value });
}

void SymbolTable::remove(std::string name)
{
    this->symbols.erase(name);
}
