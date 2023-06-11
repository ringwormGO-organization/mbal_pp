/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <variant>

#include "Context.hpp"
#include "Error.hpp"
#include "RTResult.hpp"
#include "Nodes.hpp"
#include "Values.hpp"

#include "define.hpp"

class Interpreter
{
    public:
        Interpreter();
        ~Interpreter();

        std::shared_ptr<RTResult> visit(ALL_VARIANT node, std::shared_ptr<Context> context);

        std::shared_ptr<RTResult> visit_NumberNode(ALL_VARIANT node, std::shared_ptr<Context> context);
        std::shared_ptr<RTResult> visit_BinaryOpNode(ALL_VARIANT node, std::shared_ptr<Context> context);
        std::shared_ptr<RTResult> visit_UnaryOpNode(ALL_VARIANT node, std::shared_ptr<Context> context);
};
