/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <any>
#include <iostream>
#include <map>
#include <string>
#include <variant>

#include "Values.hpp"

class SymbolTable
{
    public:
        SymbolTable() {};
        virtual ~SymbolTable() {};

        std::variant<std::shared_ptr<Number>, std::nullptr_t> get_value(std::string name);
        void set(std::string name, std::variant<std::shared_ptr<Number>, std::nullptr_t> value);
        void remove(std::string name);

    public:
        std::map<std::string, std::variant<std::shared_ptr<Number>, std::nullptr_t>> symbols;
        std::any parent = nullptr;
};
