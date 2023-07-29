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
 * Finds type in `node` variable and calls appropriate functions or throws an exception
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

    else if (std::holds_alternative<std::shared_ptr<StringNode>>(node))
    {
        return this->visit_StringNode(node, context);
    }

    else if (std::holds_alternative<std::shared_ptr<ListNode>>(node))
    {
        return this->visit_ListNode(node, context);
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

    else if (std::holds_alternative<std::shared_ptr<ForNode>>(node))
    {
        return this->visit_ForNode(node, context);
    }

    else if (std::holds_alternative<std::shared_ptr<WhileNode>>(node))
    {
        return this->visit_WhileNode(node, context);
    }

    else if (std::holds_alternative<std::shared_ptr<DoNode>>(node))
    {
        return this->visit_DoNode(node, context);
    }

    else if (std::holds_alternative<std::shared_ptr<FuncDefNode>>(node))
    {
        return this->visit_FuncDefNode(node, context);
    }

    else if (std::holds_alternative<std::shared_ptr<CallNode>>(node))
    {
        return this->visit_CallNode(node, context);
    }

    else
    {
        throw InterpreterWrongType();
    }
}

/**
 * Function handling numbers
 * @param node node
 * @param context context
*/
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

/**
 * Function handling strings
 * @param node node
 * @param context context
*/
std::shared_ptr<RTResult> Interpreter::visit_StringNode(ALL_VARIANT node, std::shared_ptr<Context> context)
{
    if (std::get<std::shared_ptr<StringNode>>(node) == nullptr)
    {
        throw NodeEmpty();
    }

    std::shared_ptr<String> str = std::make_shared<String>(std::get<std::shared_ptr<StringNode>>(node)->tok.value, context, std::get<std::shared_ptr<StringNode>>(node)->pos_start, std::get<std::shared_ptr<StringNode>>(node)->pos_end);

    std::shared_ptr<RTResult> rt_result = std::make_shared<RTResult>();
    return rt_result->success(str);
}

/**
 * Function handling lists
 * @param node node
 * @param context context
*/
std::shared_ptr<RTResult> Interpreter::visit_ListNode(ALL_VARIANT node, std::shared_ptr<Context> context)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();
    std::vector<std::shared_ptr<Value>> elements;

    for (auto &&element_node : std::get<std::shared_ptr<ListNode>>(node)->element_nodes)
    {
        elements.push_back(res->register_result(this->visit(element_node, context)));
        if (res->error->error_name != "") { return res; }
    }

    return res->success(std::make_shared<List>(elements, context, std::get<std::shared_ptr<ListNode>>(node)->pos_start, std::get<std::shared_ptr<ListNode>>(node)->pos_end));
}

/**
 * Function handling variable access
 * @param node node
 * @param context context
*/
std::shared_ptr<RTResult> Interpreter::visit_VarAccessNode(ALL_VARIANT node, std::shared_ptr<Context> context)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();

    std::string var_name = std::get<std::shared_ptr<VarAccessNode>>(node)->var_name_tok.value;
    std::variant<std::shared_ptr<Value>, std::nullptr_t> value = context->symbol_table->get_value(var_name);

    std::string details = var_name += std::string(" is not defined");
    if (std::holds_alternative<std::nullptr_t>(value))
    {
        return res->failure(std::make_shared<RTError> (
            std::get<std::shared_ptr<VarAccessNode>>(node)->pos_start, std::get<std::shared_ptr<VarAccessNode>>(node)->pos_end,
            details,
            context
        ));
    }

    std::shared_ptr<Value> new_value = std::get<std::shared_ptr<Value>>(value)->copy();

    new_value->pos_start = std::get<std::shared_ptr<VarAccessNode>>(node)->pos_start;
    new_value->pos_end = std::get<std::shared_ptr<VarAccessNode>>(node)->pos_end;
    new_value->context = context;

    return res->success(new_value);
}

/**
 * Function handling creation of variable
 * @param node node
 * @param context context
*/
std::shared_ptr<RTResult> Interpreter::visit_VarAssignNode(ALL_VARIANT node, std::shared_ptr<Context> context)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();

    std::string var_name = std::get<std::shared_ptr<VarAssignNode>>(node)->var_name_tok.value;
    std::shared_ptr<Value> value = res->register_result(this->visit(std::get<std::shared_ptr<VarAssignNode>>(node)->value_node, context));

    if (res->error->error_name != "") { return res; }

    context->symbol_table->set(var_name, value);
    return res->success(value);
}

/**
 * Function handling binary operation
 * @param node node
 * @param context context
*/
std::shared_ptr<RTResult> Interpreter::visit_BinaryOpNode(ALL_VARIANT node, std::shared_ptr<Context> context)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();

    std::shared_ptr<Value> left = res->register_result(this->visit(std::get<std::shared_ptr<BinOpNode>>(node)->left_node, context));
    if (res->error->error_name != "") { return res; }

    std::shared_ptr<Value> right = res->register_result(this->visit(std::get<std::shared_ptr<BinOpNode>>(node)->right_node, context));
    if (res->error->error_name != "") { return res; }

    if (std::holds_alternative<std::shared_ptr<BinOpNode>>(node))
    {
        std::shared_ptr<Error> error = std::make_shared<NoError>();
        std::shared_ptr<Value> result;

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

/**
 * Function handling unary operation
 * @param node node
 * @param context context
*/
std::shared_ptr<RTResult> Interpreter::visit_UnaryOpNode(ALL_VARIANT node, std::shared_ptr<Context> context)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();

    std::shared_ptr<Value> number = res->register_result(this->visit(std::get<std::shared_ptr<UnaryOpNode>>(node)->node, context));
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

/**
 * Function handling IF statment
 * @param node node
 * @param context context
*/
std::shared_ptr<RTResult> Interpreter::visit_IfNode(ALL_VARIANT node, std::shared_ptr<Context> context)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();

    bool _should_return_null;

    for (auto const& [condition, expr, should_return_null] : std::get<std::shared_ptr<IfNode>>(node)->cases.first)
    {
        _should_return_null = should_return_null;

        std::shared_ptr<Value> condition_value = res->register_result(this->visit(condition, context));
        if (res->error->error_name != "") { return res; }

        if (condition_value->is_true())
        {
            std::shared_ptr<Value> expr_value = res->register_result(this->visit(expr, context));
            if (res->error->error_name != "") { return res; }

            return res->success(should_return_null ? std::make_shared<Number>(0) : expr_value);
        }
    }

    if (!std::get<std::shared_ptr<IfNode>>(node)->is_else_case_empty)
    {
        std::pair<ALL_VARIANT, bool> _expr = std::get<std::shared_ptr<IfNode>>(node)->else_case;

        std::shared_ptr<Value> expr_value = res->register_result(this->visit(_expr.first, context));
        if (res->error->error_name != "") { return res; }

        return res->success(_should_return_null ? std::make_shared<Number>(0) : expr_value);
    }

    return res->success(std::make_shared<Number>(0));
}

/**
 * Function handling FOR statment
 * @param node node
 * @param context context
*/
std::shared_ptr<RTResult> Interpreter::visit_ForNode(ALL_VARIANT node, std::shared_ptr<Context> context)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();
    std::vector<std::shared_ptr<Value>> elements;

    std::shared_ptr<Number> start_value = std::dynamic_pointer_cast<Number>(res->register_result(this->visit(std::get<std::shared_ptr<ForNode>>(node)->start_value_node, context)));
    if (res->error->error_name != "") { return res; }

    std::shared_ptr<Number> end_value = std::dynamic_pointer_cast<Number>(res->register_result(this->visit(std::get<std::shared_ptr<ForNode>>(node)->end_value_node, context)));
    if (res->error->error_name != "") { return res; }

    std::shared_ptr<Number> step_value = std::make_shared<Number>(1);
    if (std::get<std::shared_ptr<ForNode>>(node)->step_value_node != ALL_VARIANT{}) /* check if step_value_node is empty */
    {
        step_value = std::dynamic_pointer_cast<Number>(res->register_result(this->visit(std::get<std::shared_ptr<ForNode>>(node)->step_value_node, context)));
        if (res->error->error_name != "") { return res; }
    }

    double i = start_value->value;
    auto condition = [](double step_value, double _i, double end_value_value) 
    {
        if (step_value >= 0)
        {
            return _i < end_value_value;
        }

        return _i > end_value_value;
    };

    while (condition(step_value->value, i, end_value->value))
    {
        context->symbol_table->set(std::get<std::shared_ptr<ForNode>>(node)->var_name_tok.value, std::make_shared<Number>(i));

        i += step_value->value;

        elements.push_back(res->register_result(this->visit(std::get<std::shared_ptr<ForNode>>(node)->body_node, context)));
        if (res->error->error_name != "") { return res; }
    }

    return res->success(std::get<std::shared_ptr<ForNode>>(node)->should_return_null ? std::static_pointer_cast<Value>(std::make_shared<Number>(0)) : std::make_shared<List> (
        elements, context, std::get<std::shared_ptr<ForNode>>(node)->pos_start, std::get<std::shared_ptr<ForNode>>(node)->pos_end
    ));
}

/**
 * Function handling WHILE statment
 * @param node node
 * @param context context
*/
std::shared_ptr<RTResult> Interpreter::visit_WhileNode(ALL_VARIANT node, std::shared_ptr<Context> context)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();
    std::vector<std::shared_ptr<Value>> elements;

    while (true)
    {
        std::shared_ptr<Number> condition = std::dynamic_pointer_cast<Number>(res->register_result(this->visit(std::get<std::shared_ptr<WhileNode>>(node)->condition_node, context)));
        if (res->error->error_name != "") { return res; }

        if (!condition->is_true()) break;

        elements.push_back(res->register_result(this->visit(std::get<std::shared_ptr<WhileNode>>(node)->body_node, context)));
        if (res->error->error_name != "") { return res; }
    }

    return res->success((std::get<std::shared_ptr<WhileNode>>(node)->should_return_null) ? std::static_pointer_cast<Value>(std::make_shared<Number>(0)) : std::make_shared<List> (
        elements, context, std::get<std::shared_ptr<WhileNode>>(node)->pos_start, std::get<std::shared_ptr<WhileNode>>(node)->pos_end
    ));
}

/**
 * Function handling DO statment, very similar to `visit_WhileNode()` function
 * @param node node
 * @param context context
*/
std::shared_ptr<RTResult> Interpreter::visit_DoNode(ALL_VARIANT node, std::shared_ptr<Context> context)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();
    std::vector<std::shared_ptr<Value>> elements;

    while (true)
    {
        std::shared_ptr<Number> condition = std::dynamic_pointer_cast<Number>(res->register_result(this->visit(std::get<std::shared_ptr<DoNode>>(node)->condition_node, context)));
        if (res->error->error_name != "") { return res; }

        if (!condition->is_true()) break;

        elements.push_back(res->register_result(this->visit(std::get<std::shared_ptr<DoNode>>(node)->body_node, context)));
        if (res->error->error_name != "") { return res; }
    }

    return res->success(std::get<std::shared_ptr<DoNode>>(node)->should_return_null ? std::static_pointer_cast<Value>(std::make_shared<Number>(0)) : std::make_shared<List> (
        elements, context, std::get<std::shared_ptr<DoNode>>(node)->pos_start, std::get<std::shared_ptr<DoNode>>(node)->pos_end
    ));
}

/**
 * Function handling functions
 * @param node node
 * @param context context
*/
std::shared_ptr<RTResult> Interpreter::visit_FuncDefNode(ALL_VARIANT node, std::shared_ptr<Context> context)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();

    std::string func_name = std::get<std::shared_ptr<FuncDefNode>>(node)->var_name_tok.value;
    ALL_VARIANT body_node = std::get<std::shared_ptr<FuncDefNode>>(node)->body_node;

    std::vector<std::string> arg_names;
    for (auto &&i : std::get<std::shared_ptr<FuncDefNode>>(node)->arg_name_toks)
    {
        arg_names.push_back(i.value);
    }
    
    std::shared_ptr<Function> func_value = std::make_shared<Function>(func_name, body_node, arg_names, std::get<std::shared_ptr<FuncDefNode>>(node)->should_return_null);

    func_value->context = context;
    func_value->pos_start = std::get<std::shared_ptr<FuncDefNode>>(node)->pos_start;
    func_value->pos_end = std::get<std::shared_ptr<FuncDefNode>>(node)->pos_end;

    if (std::get<std::shared_ptr<FuncDefNode>>(node)->var_name_tok.type != TT::NUL)
    {
        context->symbol_table->set(func_name, func_value);
    }

    return res->success(func_value);
}

/**
 * Function handling function call
 * @param node node
 * @param context context
*/
std::shared_ptr<RTResult> Interpreter::visit_CallNode(ALL_VARIANT node, std::shared_ptr<Context> context)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();
    std::vector<std::shared_ptr<Value>> args;

    std::shared_ptr<BuiltInFunction> value_to_call = std::dynamic_pointer_cast<BuiltInFunction>(res->register_result(this->visit(std::get<std::shared_ptr<CallNode>>(node)->node_to_call, context)));
    if (res->error->error_name != "") { return res; }

    value_to_call = std::dynamic_pointer_cast<BuiltInFunction>(value_to_call->copy());
    value_to_call->context = context;

    value_to_call->pos_start = std::get<std::shared_ptr<CallNode>>(node)->pos_start;
    value_to_call->pos_end = std::get<std::shared_ptr<CallNode>>(node)->pos_end;

    for (auto &&arg_node : std::get<std::shared_ptr<CallNode>>(node)->arg_nodes)
    {
        args.push_back(res->register_result(this->visit(arg_node, context)));
        if (res->error->error_name != "") { return res; }
    }

    std::shared_ptr<Value> return_value = res->register_result(value_to_call->execute(args));
    if (res->error->error_name != "") { return res; }

    return_value = return_value->copy();
    
    return_value->pos_start = std::get<std::shared_ptr<CallNode>>(node)->pos_start;
    return_value->pos_end = std::get<std::shared_ptr<CallNode>>(node)->pos_end;
    return_value->context = context;

    return res->success(return_value);
}
