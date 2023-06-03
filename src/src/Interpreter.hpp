/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <variant>

#include "Context.hpp"
#include "Error.hpp"
#include "Nodes.hpp"
#include "Values.hpp"

#include "define.hpp"

class Interpreter
{
    public:
        Interpreter(/* args */);
        ~Interpreter();

        std::shared_ptr<Number> visit(ALL_VARIANT node, Context context);

        std::shared_ptr<Number> visit_NumberNode(ALL_VARIANT node, Context context);
        std::shared_ptr<Number> visit_BinaryOpNode(ALL_VARIANT node, Context context);
        std::shared_ptr<Number> visit_UnaryOpNode(ALL_VARIANT node, Context context);
};
