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

/* Macro handling returned derived classes from Error class */
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

/**
 * Class which runs the program
*/
class Interpreter
{
    public:
        Interpreter();
        ~Interpreter();

        /**
         * Finds type in `node` variable and calls appropriate functions or throws an exception
         * @param node node
         * @param context context
         * @return std::shared_ptr<RTResult>
        */
        std::shared_ptr<RTResult> visit(ALL_VARIANT node, std::shared_ptr<Context> context);

        /**
         * Function handling numbers
         * @param node node
         * @param context context
        */
        std::shared_ptr<RTResult> visit_NumberNode(ALL_VARIANT node, std::shared_ptr<Context> context);

        /**
         * Function handling strings
         * @param node node
         * @param context context
        */
        std::shared_ptr<RTResult> visit_StringNode(ALL_VARIANT node, std::shared_ptr<Context> context);

        /**
         * Function handling lists
         * @param node node
         * @param context context
        */
        std::shared_ptr<RTResult> visit_ListNode(ALL_VARIANT node, std::shared_ptr<Context> context);

        /**
         * Function handling variable access
         * @param node node
         * @param context context
        */
        std::shared_ptr<RTResult> visit_VarAccessNode(ALL_VARIANT node, std::shared_ptr<Context> context);

        /**
         * Function handling creation of variable
         * @param node node
         * @param context context
        */
        std::shared_ptr<RTResult> visit_VarAssignNode(ALL_VARIANT node, std::shared_ptr<Context> context);

        /**
         * Function handling binary operation
         * @param node node
         * @param context context
        */
        std::shared_ptr<RTResult> visit_BinaryOpNode(ALL_VARIANT node, std::shared_ptr<Context> context);

        /**
         * Function handling unary operation
         * @param node node
         * @param context context
        */
        std::shared_ptr<RTResult> visit_UnaryOpNode(ALL_VARIANT node, std::shared_ptr<Context> context);

        /**
         * Function handling IF statment
         * @param node node
         * @param context context
        */
        std::shared_ptr<RTResult> visit_IfNode(ALL_VARIANT node, std::shared_ptr<Context> context);

        /**
         * Function handling FOR statment
         * @param node node
         * @param context context
        */
        std::shared_ptr<RTResult> visit_ForNode(ALL_VARIANT node, std::shared_ptr<Context> context);

        /**
         * Function handling WHILE statment
         * @param node node
         * @param context context
        */
        std::shared_ptr<RTResult> visit_WhileNode(ALL_VARIANT node, std::shared_ptr<Context> context);

        /**
         * Function handling DO statment, very similar to `visit_WhileNode()` function
         * @param node node
         * @param context context
        */
        std::shared_ptr<RTResult> visit_DoNode(ALL_VARIANT node, std::shared_ptr<Context> context);

        /**
         * Function handling functions
         * @param node node
         * @param context context
        */
        std::shared_ptr<RTResult> visit_FuncDefNode(ALL_VARIANT node, std::shared_ptr<Context> context);

        /**
         * Function handling function call
         * @param node node
         * @param context context
        */
        std::shared_ptr<RTResult> visit_CallNode(ALL_VARIANT node, std::shared_ptr<Context> context);
};
