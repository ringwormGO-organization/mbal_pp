/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Interpreter.hpp"

#include "SymbolTable.hpp"

Interpreter::Interpreter()
{

}

Interpreter::~Interpreter()
{

}

/**
 * Finds type in `node` variable and calls appropriate functions (down below) or throws an exception
 * @param node node
 * @param context context
 * @return std::shared_ptr<RTResult>
*/
std::shared_ptr<RTResult> Interpreter::visit(ALL_VARIANT node, std::shared_ptr<Context> context)
{
    if (std::holds_alternative<std::shared_ptr<NumberNode>>(node))
    {
        return this->visit_NumberNode(node, context);
    }

    else if (std::holds_alternative<std::shared_ptr<VarAccessNode>>(node))
    {
        return this->visit_VarAccessNode(node, context);
    }

    else if (std::holds_alternative<std::shared_ptr<VarAssignNode>>(node))
    {
        return this->visit_VarAssignNode(node, context);
    }

    else if (std::holds_alternative<std::shared_ptr<BinOpNode>>(node))
    {
        return this->visit_BinaryOpNode(node, context);
    }

    else if (std::holds_alternative<std::shared_ptr<UnaryOpNode>>(node))
    {
        return this->visit_UnaryOpNode(node, context);
    }

    else if (std::holds_alternative<std::shared_ptr<IfNode>>(node))
    {
        return this->visit_IfNode(node, context);
    }

    else
    {
        throw InterpreterWrongType();
    }
}

std::shared_ptr<RTResult> Interpreter::visit_NumberNode(ALL_VARIANT node, std::shared_ptr<Context> context)
{
    if (std::get<0>(node) == nullptr)
    {
        throw NodeEmpty();
    }

    std::shared_ptr<Number> number = std::make_shared<Number>(std::stod(std::get<0>(node)->tok.value), context, std::get<0>(node)->pos_start, std::get<0>(node)->pos_end);

    std::shared_ptr<RTResult> rt_result = std::make_shared<RTResult>();
    return rt_result->success(number);
}

std::shared_ptr<RTResult> Interpreter::visit_VarAccessNode(ALL_VARIANT node, std::shared_ptr<Context> context)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();

    std::string var_name = std::get<std::shared_ptr<VarAccessNode>>(node)->var_name_tok.value;
    std::variant<std::shared_ptr<Number>, std::nullptr_t> value = context->symbol_table->get_value(var_name);

    std::string details = var_name += std::string(" is not defined");
    if (std::holds_alternative<std::nullptr_t>(value))
    {
        return res->failure(std::make_shared<RTError> (
            std::get<std::shared_ptr<VarAccessNode>>(node)->pos_start, std::get<std::shared_ptr<VarAccessNode>>(node)->pos_end,
            details,
            context
        ));
    }

    std::get<std::shared_ptr<Number>>(value)->pos_start = std::get<std::shared_ptr<VarAccessNode>>(node)->pos_start;
    std::get<std::shared_ptr<Number>>(value)->pos_end = std::get<std::shared_ptr<VarAccessNode>>(node)->pos_end;

    std::shared_ptr<Number> new_value = std::get<std::shared_ptr<Number>>(value);
    return res->success(new_value);
}

std::shared_ptr<RTResult> Interpreter::visit_VarAssignNode(ALL_VARIANT node, std::shared_ptr<Context> context)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();

    std::string var_name = std::get<std::shared_ptr<VarAssignNode>>(node)->var_name_tok.value;
    std::shared_ptr<Number> value = res->register_result(this->visit(std::get<std::shared_ptr<VarAssignNode>>(node)->value_node, context));

    if (res->error->error_name != "") { return res; }

    context->symbol_table->set(var_name, value);
    return res->success(value);
}

std::shared_ptr<RTResult> Interpreter::visit_BinaryOpNode(ALL_VARIANT node, std::shared_ptr<Context> context)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();

    std::shared_ptr<Number> left = res->register_result(this->visit(std::get<std::shared_ptr<BinOpNode>>(node)->left_node, context));
    if (res->error->error_name != "") { return res; }

    std::shared_ptr<Number> right = res->register_result(this->visit(std::get<std::shared_ptr<BinOpNode>>(node)->right_node, context));
    if (res->error->error_name != "") { return res; }

    if (std::holds_alternative<std::shared_ptr<BinOpNode>>(node))
    {
        std::shared_ptr<Error> error = std::make_shared<NoError>();
        std::shared_ptr<Number> result;

        if (std::get<std::shared_ptr<BinOpNode>>(node)->op_tok.type == TT::PLUS)
        {
            auto [temp_result, temp_error] = left->added_to(right);

            result = temp_result;
            SET_ERROR
        }

        else if (std::get<std::shared_ptr<BinOpNode>>(node)->op_tok.type == TT::MINUS)
        {
            auto [temp_result, temp_error] = left->subbed_by(right);

            result = temp_result;
            SET_ERROR
        }

        else if (std::get<std::shared_ptr<BinOpNode>>(node)->op_tok.type == TT::MUL)
        {
            auto [temp_result, temp_error] = left->multed_by(right);

            result = temp_result;
            SET_ERROR
        }

        else if (std::get<std::shared_ptr<BinOpNode>>(node)->op_tok.type == TT::DIV)
        {
            auto [temp_result, temp_error] = left->dived_by(right);

            result = temp_result;
            SET_ERROR
        }

        else if (std::get<std::shared_ptr<BinOpNode>>(node)->op_tok.type == TT::POW)
        {
            auto [temp_result, temp_error] = left->powed_by(right);

            result = temp_result;
            SET_ERROR
        }

        else if (std::get<std::shared_ptr<BinOpNode>>(node)->op_tok.type == TT::EE)
        {
            auto [temp_result, temp_error] = left->get_comparison_eq(right);

            result = temp_result;
            SET_ERROR
        }

        else if (std::get<std::shared_ptr<BinOpNode>>(node)->op_tok.type == TT::NE)
        {
            auto [temp_result, temp_error] = left->get_comparison_ne(right);

            result = temp_result;
            SET_ERROR
        }

        else if (std::get<std::shared_ptr<BinOpNode>>(node)->op_tok.type == TT::LT)
        {
            auto [temp_result, temp_error] = left->get_comparison_lt(right);

            result = temp_result;
            SET_ERROR 
        }

        else if (std::get<std::shared_ptr<BinOpNode>>(node)->op_tok.type == TT::GT)
        {
            auto [temp_result, temp_error] = left->get_comparison_gt(right);

            result = temp_result;
            SET_ERROR
        }

        else if (std::get<std::shared_ptr<BinOpNode>>(node)->op_tok.type == TT::LTE)
        {
            auto [temp_result, temp_error] = left->get_comparison_lte(right);

            result = temp_result;
            SET_ERROR
        }

        else if (std::get<std::shared_ptr<BinOpNode>>(node)->op_tok.type == TT::GTE)
        {
            auto [temp_result, temp_error] = left->get_comparison_gte(right);

            result = temp_result;
            SET_ERROR
        }

        else if (std::get<std::shared_ptr<BinOpNode>>(node)->op_tok.matches(TT::KEYWORD, KEYWORDS[1]))
        {
            auto [temp_result, temp_error] = left->anded_by(right);

            result = temp_result;
            SET_ERROR
        }

        else if (std::get<std::shared_ptr<BinOpNode>>(node)->op_tok.matches(TT::KEYWORD, KEYWORDS[2]))
        {
            auto [temp_result, temp_error] = left->ored_by(right);

            result = temp_result;
            SET_ERROR
        }

        if (error->error_name != "")
        {
            return res->failure(error);
        }

        else
        {
            result->pos_start = std::get<std::shared_ptr<BinOpNode>>(node)->pos_start;
            result->pos_end = std::get<std::shared_ptr<BinOpNode>>(node)->pos_end;

            return res->success(result);
        }
    }

    throw InterpreterWrongType();
    return nullptr;
}

std::shared_ptr<RTResult> Interpreter::visit_UnaryOpNode(ALL_VARIANT node, std::shared_ptr<Context> context)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();

    std::shared_ptr<Number> number = res->register_result(this->visit(std::get<std::shared_ptr<UnaryOpNode>>(node)->node, context));
    if (res->error->error_name != "") { return res; }

    std::shared_ptr<Error> error = std::make_shared<NoError>();

    if (std::holds_alternative<std::shared_ptr<UnaryOpNode>>(node))
    {
        if (std::get<std::shared_ptr<UnaryOpNode>>(node)->op_tok.type == TT::MINUS)
        {
            auto [temp_number, temp_error] = number->multed_by(std::make_shared<Number>(-1));

            number = temp_number;
            SET_ERROR
        }

        else if (std::get<std::shared_ptr<UnaryOpNode>>(node)->op_tok.matches(TT::KEYWORD, "NOT"))
        {
            auto [temp_number, temp_error] = number->notted_by();

            number = temp_number;
            SET_ERROR
        }

        if (error->error_name != "")
        {
            return res->failure(error);
        }

        else
        {
            number->pos_start = std::get<std::shared_ptr<UnaryOpNode>>(node)->pos_start;
            number->pos_end = std::get<std::shared_ptr<UnaryOpNode>>(node)->pos_end;

            return res->success(number);
        }
    }

    throw InterpreterWrongType();
    return nullptr;
}

std::shared_ptr<RTResult> Interpreter::visit_IfNode(ALL_VARIANT node, std::shared_ptr<Context> context)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();

    for (auto const& [condition, expr] : std::get<std::shared_ptr<IfNode>>(node)->cases)
    {
        std::shared_ptr<Number> condition_value = res->register_result(this->visit(condition, context));
        if (res->error->error_name != "") { return res; }

        if (condition_value->is_true())
        {
            std::shared_ptr<Number> expr_value = res->register_result(this->visit(expr, context));
            if (res->error->error_name != "") { return res; }

            return res->success(expr_value);
        }
    }

    bool is_empty = (std::get<std::shared_ptr<IfNode>>(node)->else_case == ALL_VARIANT{});
    if (!is_empty)
    {
        std::shared_ptr<Number> else_value = res->register_result(this->visit(std::get<std::shared_ptr<IfNode>>(node)->else_case, context));
        if (res->error->error_name != "") { return res; }

        return res->success(else_value);
    }

    return res->success(nullptr);
}
