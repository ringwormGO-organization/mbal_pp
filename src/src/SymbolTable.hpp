/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <map>
#include <string>
#include <variant>

#include "Context.hpp"
#include "Position.hpp"
#include "Values.hpp"

class Context;

class SymbolTable
{
    public:
        SymbolTable(std::shared_ptr<Context> context=nullptr);
        virtual ~SymbolTable();

        std::variant<std::shared_ptr<Value>, std::nullptr_t> get_value(std::string name);
        void set(std::string name, std::variant<std::shared_ptr<Value>, std::nullptr_t> value);
        void remove(std::string name);

    public:
        std::map<std::string, std::variant<std::shared_ptr<Value>, std::nullptr_t>> symbols;
        std::shared_ptr<Context> parent;
};
