/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <string>
#include <type_traits>
#include <variant>

#include "Context.hpp"
#include "Error.hpp"
#include "Nodes.hpp"

class Interpreter
{
    public:
        Interpreter(/* args */);
        ~Interpreter();

        void visit(std::variant<NumberNode*, BinOpNode*, UnaryOpNode*> node, Context context);

        void visit_NumberNode(std::variant<NumberNode*, BinOpNode*, UnaryOpNode*> node, Context context);
        void visit_BinaryOpNode(std::variant<NumberNode*, BinOpNode*, UnaryOpNode*> node, Context context);
        void visit_UnaryOpNode(std::variant<NumberNode*, BinOpNode*, UnaryOpNode*> node, Context context);
};
