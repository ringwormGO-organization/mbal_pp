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

#define SET_ERROR \
    if (auto illegal_char_error = std::dynamic_pointer_cast<IllegalCharError>(temp_error)) \
    { \
        error = std::make_shared<IllegalCharError>(illegal_char_error->pos_start, illegal_char_error->pos_end, illegal_char_error->details); \
    } \
    else if (auto invalid_syntax_error = std::dynamic_pointer_cast<InvalidSyntaxError>(temp_error)) \
    { \
        error = std::make_shared<InvalidSyntaxError>(invalid_syntax_error->pos_start, invalid_syntax_error->pos_end, invalid_syntax_error->details); \
    } \
    else if (auto rt_error = std::dynamic_pointer_cast<RTError>(temp_error)) \
    { \
        error = std::make_shared<RTError>(rt_error->pos_start, rt_error->pos_end, rt_error->details, rt_error->context); \
    } \
    else if (auto no_error = std::dynamic_pointer_cast<NoError>(temp_error)) \
    { \
        error = std::make_shared<NoError>(); \
    } \
    else \
    { \
        error = std::make_shared<Error>(rt_error->pos_start, rt_error->pos_end, "", ""); \
    } \

class Interpreter
{
    public:
        Interpreter();
        ~Interpreter();

        std::shared_ptr<RTResult> visit(ALL_VARIANT node, std::shared_ptr<Context> context);

        std::shared_ptr<RTResult> visit_NumberNode(ALL_VARIANT node, std::shared_ptr<Context> context);
        std::shared_ptr<RTResult> visit_VarAccessNode(ALL_VARIANT node, std::shared_ptr<Context> context);
        std::shared_ptr<RTResult> visit_VarAssignNode(ALL_VARIANT node, std::shared_ptr<Context> context);
        std::shared_ptr<RTResult> visit_BinaryOpNode(ALL_VARIANT node, std::shared_ptr<Context> context);
        std::shared_ptr<RTResult> visit_UnaryOpNode(ALL_VARIANT node, std::shared_ptr<Context> context);
};
