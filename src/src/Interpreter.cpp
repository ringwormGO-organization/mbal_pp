/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Interpreter.hpp"

Interpreter::Interpreter()
{

}

Interpreter::~Interpreter()
{

}

void Interpreter::visit(std::variant<NumberNode*, BinOpNode*, UnaryOpNode*> node, Context context)
{
    std::string method_name = "visit_";
    if (std::holds_alternative<NumberNode*>(node))
    {
        method_name += "NumberNode";
    }

    else if (std::holds_alternative<BinOpNode*>(node))
    {
        method_name += "BinOpNode";
    }

    else if (std::holds_alternative<UnaryOpNode*>(node))
    {
        method_name += "UnaryOpNode";
    }

    else
    {
        throw InterpreterWrongType();
    }

    if (method_name == "visit_NumberNode")
    {
        return this->visit_NumberNode(node, context);
    }

    else if (method_name == "visit_BinOpNode")
    {
        return this->visit_BinaryOpNode(node, context);
    }

    else if (method_name == "visit_UnaryOpNode")
    {
        return this->visit_UnaryOpNode(node, context);
    }

    else
    {
        throw InterpreterWrongType();
    }

    return;
}

void Interpreter::visit_NumberNode(std::variant<NumberNode*, BinOpNode*, UnaryOpNode*> node, Context context)
{
    std::cout << "Found number node!\n";
}

void Interpreter::visit_BinaryOpNode(std::variant<NumberNode*, BinOpNode*, UnaryOpNode*> node, Context context)
{
    std::cout << "Found binary node!\n";
}

void Interpreter::visit_UnaryOpNode(std::variant<NumberNode*, BinOpNode*, UnaryOpNode*> node, Context context)
{
    std::cout << "Found unary op node!\n";
}
