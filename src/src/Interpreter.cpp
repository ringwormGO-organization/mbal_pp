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

std::shared_ptr<RTResult> Interpreter::visit(ALL_VARIANT node, Context context)
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

std::shared_ptr<RTResult> Interpreter::visit_NumberNode(ALL_VARIANT node, Context context)
{
    std::shared_ptr<Number> number = std::make_shared<Number>(std::stol(std::get<0>(node)->tok.value), std::get<0>(node)->pos_start, std::get<0>(node)->pos_end);

    std::shared_ptr<RTResult> rt_result = std::make_shared<RTResult>();
    return rt_result->success(number);
}

std::shared_ptr<RTResult> Interpreter::visit_BinaryOpNode(ALL_VARIANT node, Context context)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();

    std::shared_ptr<Number> left = res->register_result(this->visit(std::get<1>(node)->left_node, context));
    if (res->error.error_name != "") { return res; }

    std::shared_ptr<Number> right = res->register_result(this->visit(std::get<1>(node)->right_node, context));
    if (res->error.error_name != "") { return res; }

    if (std::holds_alternative<std::shared_ptr<BinOpNode>>(node))
    {
        Error error = NoError();
        std::shared_ptr<Number> result;

        if (std::get<1>(node)->op_tok.type == TT::PLUS)
        {
            auto [temp_result, temp_error] = left->added_to(right);

            result = temp_result;
            error = temp_error;
        }

        else if (std::get<1>(node)->op_tok.type == TT::MINUS)
        {
            auto [temp_result, temp_error] = left->subbed_by(right);

            result = temp_result;
            error = temp_error;
        }

        else if (std::get<1>(node)->op_tok.type == TT::MUL)
        {
            auto [temp_result, temp_error] = left->multed_by(right);

            result = temp_result;
            error = temp_error;
        }

        else if (std::get<1>(node)->op_tok.type == TT::DIV)
        {
            auto [temp_result, temp_error] = left->dived_by(right);

            result = temp_result;
            error = temp_error;
        }

        else
        {
            // Handle other cases if necessary
        }

        if (error.error_name != "")
        {
            return res->failure(error);
        }

        else
        {
            result->pos_start = std::get<1>(node)->pos_start;
            result->pos_end = std::get<1>(node)->pos_end;

            return res->success(result);
        }
    }
}

std::shared_ptr<RTResult> Interpreter::visit_UnaryOpNode(ALL_VARIANT node, Context context)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();

    std::shared_ptr<Number> number = res->register_result(this->visit(std::get<2>(node)->node, context));
    if (res->error.error_name != "") { return res; }

    Error error = NoError();

    if (std::holds_alternative<std::shared_ptr<UnaryOpNode>>(node))
    {
        if (std::get<2>(node)->op_tok.type == TT::MINUS)
        {
            auto [temp_number, temp_error] = number->multed_by(std::make_shared<Number>(-1));

            number = temp_number;
            error = temp_error;
        }

        if (error.error_name != "")
        {
            return res->failure(error);
        }

        else
        {
            number->pos_start = std::get<2>(node)->pos_start;
            number->pos_end = std::get<2>(node)->pos_end;

            return res->success(number);
        }
    }
}
