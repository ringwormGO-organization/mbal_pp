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

std::shared_ptr<Number> Interpreter::visit(ALL_VARIANT node, Context context)
{
    std::string method_name = "visit_";
    if (std::holds_alternative<std::shared_ptr<NumberNode>>(node))
    {
        method_name += "NumberNode";
    }

    else if (std::holds_alternative<std::shared_ptr<BinOpNode>>(node))
    {
        method_name += "BinOpNode";
    }

    else if (std::holds_alternative<std::shared_ptr<UnaryOpNode>>(node))
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
}

std::shared_ptr<Number> Interpreter::visit_NumberNode(ALL_VARIANT node, Context context)
{
    Number number(std::stol(std::get<0>(node)->tok.value), std::get<0>(node)->pos_start, std::get<0>(node)->pos_end);
    return std::make_shared<Number>(number);
}

std::shared_ptr<Number> Interpreter::visit_BinaryOpNode(ALL_VARIANT node, Context context)
{
    std::shared_ptr<Number> left = this->visit(std::get<1>(node)->left_node, context);
    std::shared_ptr<Number> right = this->visit(std::get<1>(node)->right_node, context);

    if (std::holds_alternative<std::shared_ptr<BinOpNode>>(node))
    {
        std::shared_ptr<Number> result;
        if (std::get<1>(node)->op_tok.type == TT::PLUS)
        {
            result = left->added_to(right);
        }

        else if (std::get<1>(node)->op_tok.type == TT::MINUS)
        {
            result = left->subbed_by(right);
        }

        else if (std::get<1>(node)->op_tok.type == TT::MUL)
        {
            result = left->multed_by(right);
        }

        else if (std::get<1>(node)->op_tok.type == TT::DIV)
        {
            result = left->dived_by(right);
        }

        else
        {
            // Handle other cases if necessary
        }

        result->pos_start = std::get<1>(node)->pos_start;
        result->pos_end = std::get<1>(node)->pos_end;

        return result;
    }
}

std::shared_ptr<Number> Interpreter::visit_UnaryOpNode(ALL_VARIANT node, Context context)
{
    std::shared_ptr<Number> number = this->visit(std::get<2>(node)->node, context);

    if (std::holds_alternative<std::shared_ptr<UnaryOpNode>>(node))
    {
        if (std::get<2>(node)->op_tok.type == TT::MINUS)
        {
            number = number->multed_by(std::make_shared<Number>(-1));
        }

        number->pos_start = std::get<2>(node)->pos_start;
        number->pos_end = std::get<2>(node)->pos_end;

        return number;
    }
}
