/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Values.hpp"

#include "Context.hpp"
#include "Error.hpp"
#include "Interpreter.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Position.hpp"
#include "RTResult.hpp"
#include "SymbolTable.hpp"
#include "Token.hpp"

template <typename T>
void extend(std::vector<T>& vec, const std::vector<T>& elements) 
{
    vec.insert(vec.end(), elements.begin(), elements.end());
}

/**
 * Converts std::any to ALL_VARIANT, from main.cpp
 * @param node - node to be converted
 * @return ALL_VARIANT
*/
ALL_VARIANT convert_2(std::any node)
{
    ALL_VARIANT return_value;

    if (node.type() == typeid(std::shared_ptr<NumberNode>)) 
    {
        return_value = std::any_cast<std::shared_ptr<NumberNode>>(node);
    }

    else if (node.type() == typeid(std::shared_ptr<StringNode>)) 
    {
        return_value = std::any_cast<std::shared_ptr<StringNode>>(node);
    }

    else if (node.type() == typeid(std::shared_ptr<ListNode>))
    {
        return_value = std::any_cast<std::shared_ptr<ListNode>>(node);
    }

    else if (node.type() == typeid(std::shared_ptr<VarAccessNode>))
    {
        return_value = std::any_cast<std::shared_ptr<VarAccessNode>>(node);
    }

    else if (node.type() == typeid(std::shared_ptr<VarAssignNode>))
    {
        return_value = std::any_cast<std::shared_ptr<VarAssignNode>>(node);
    }

    else if (node.type() == typeid(std::shared_ptr<BinOpNode>))
    {
        return_value = std::any_cast<std::shared_ptr<BinOpNode>>(node);
    }

    else if (node.type() == typeid(std::shared_ptr<UnaryOpNode>))
    {
        return_value = std::any_cast<std::shared_ptr<UnaryOpNode>>(node);
    }

    else if (node.type() == typeid(std::shared_ptr<IfNode>))
    {
        return_value = std::any_cast<std::shared_ptr<IfNode>>(node);
    }

    else if (node.type() == typeid(std::shared_ptr<WhileNode>))
    {
        return_value = std::any_cast<std::shared_ptr<WhileNode>>(node);
    }

    else if (node.type() == typeid(std::shared_ptr<DoNode>))
    {
        return_value = std::any_cast<std::shared_ptr<DoNode>>(node);
    }

    else if (node.type() == typeid(std::shared_ptr<FuncDefNode>))
    {
        return_value = std::any_cast<std::shared_ptr<FuncDefNode>>(node);
    }

    else if (node.type() == typeid(std::shared_ptr<CallNode>))
    {
        return_value = std::any_cast<std::shared_ptr<CallNode>>(node);
    }

    else 
    {
        throw WrongAny();
        return return_value;
    }

    return return_value;
}

/**
 * Calls lexer, parser and interpreter, from main.cpp
 * @param fn name of function
 * @param text input
 * @return result (Number class) or error (Error class)
*/
std::variant<std::shared_ptr<Value>, std::shared_ptr<Error>> run_2(std::string fn, std::string text)
{
    std::shared_ptr<SymbolTable> global_symbol_table = std::make_shared<SymbolTable>();

    global_symbol_table->set(std::string("NULL"), std::make_shared<Number>(0));
    global_symbol_table->set(std::string("TRUE"), std::make_shared<Number>(1));
    global_symbol_table->set(std::string("FALSE"), std::make_shared<Number>(0));
    global_symbol_table->set(std::string("MATH_PI"), std::make_shared<Number>(M_PI));

    global_symbol_table->set(std::string("clear"), std::make_shared<BuiltInFunction>("clear"));
    global_symbol_table->set(std::string("print"), std::make_shared<BuiltInFunction>("print"));
    global_symbol_table->set(std::string("write"), std::make_shared<BuiltInFunction>("write"));
    global_symbol_table->set(std::string("write"), std::make_shared<BuiltInFunction>("len"));
    global_symbol_table->set(std::string("write"), std::make_shared<BuiltInFunction>("run"));

    /* Generate tokens */
    Lexer lexer(fn, text);
    std::tuple<std::vector<Token>, std::shared_ptr<Error>> result = lexer.make_tokens();

    if (std::get<1>(result)->error_name.size() > 0)
    {
        return std::get<1>(result);
    }

    /* Generate AST */
    Parser parser(std::get<0>(result));
    std::shared_ptr<ParseResult> ast = parser.parse();

    if (ast->error->error_name != "")
    {
        return ast->error;
    }

    /* Run program */
    Interpreter interpreter;
    std::shared_ptr<Context> context = std::make_shared<Context>("<program>");

    context->symbol_table = global_symbol_table;
    std::shared_ptr<RTResult> interpreter_result = interpreter.visit(convert_2(ast->node), context);

    if (interpreter_result->error->error_name != "")
    {
        return interpreter_result->error;
    }

    return interpreter_result->value;
}

Value::Value(std::shared_ptr<Context> context, std::shared_ptr<Position> pos_start, std::shared_ptr<Position> pos_end)
{
    if (pos_start != nullptr) { this->pos_start = pos_start; }
    if (pos_end != nullptr) { this->pos_end = pos_end; }
    if (context != nullptr) { this->context = context; }
}

Value::~Value()
{

}

/**
 * Returns value as string
 * @return std::string
*/
std::string Value::repr()
{
    return "[repr()] I am from base class `Value`, please check why I am displayed!";
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Value::added_to(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    return { nullptr, this->illegal_operation(other) };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Value::subbed_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    return { nullptr, this->illegal_operation(other) };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Value::multed_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    return { nullptr, this->illegal_operation(other) };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Value::dived_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    return { nullptr, this->illegal_operation(other) };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Value::powed_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    return { nullptr, this->illegal_operation(other) };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Value::get_comparison_eq(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    return { nullptr, this->illegal_operation(other) };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Value::get_comparison_ne(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    return { nullptr, this->illegal_operation(other) };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Value::get_comparison_lt(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    return { nullptr, this->illegal_operation(other) };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Value::get_comparison_gt(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    return { nullptr, this->illegal_operation(other) };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Value::get_comparison_lte(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    return { nullptr, this->illegal_operation(other) };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Value::get_comparison_gte(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    return { nullptr, this->illegal_operation(other) };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Value::anded_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    return { nullptr, this->illegal_operation(other) };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Value::ored_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    return { nullptr, this->illegal_operation(other) };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Value::notted_by()
{
    return { nullptr, this->illegal_operation() };
}

std::shared_ptr<RTResult> Value::execute(std::vector<std::shared_ptr<Value>> args)
{
    UNUSED(args);
    std::shared_ptr<RTResult> result = std::make_shared<RTResult>();

    return result->failure(this->illegal_operation());
}

std::shared_ptr<Value> Value::copy()
{
    throw NoCopy();
    return nullptr;
}

bool Value::is_true()
{
    return false;
}

std::shared_ptr<Error> Value::illegal_operation(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::nullptr_t>(other)) { other = std::shared_ptr<Value>(std::addressof(*this)); }

    return std::make_shared<RTError> (
        this->pos_start, this->pos_end,
        "Illegal operation",
        this->context
    );
}

/* ---------------------------------------------------------------------------- */

Number::Number(double value, std::shared_ptr<Context> context, std::shared_ptr<Position> pos_start, std::shared_ptr<Position> pos_end) : Value(context, pos_start, pos_end)
{
    this->value = value;

    if (pos_start != nullptr) { this->pos_start = pos_start; }
    if (pos_end != nullptr) { this->pos_end = pos_end; }
    if (context != nullptr ) { this->context = context; }
}

Number::~Number()
{

}

/**
 * Returns value as string
 * @return std::string
*/
std::string Number::repr()
{
    std::string formatted_number = std::to_string(this->value);
    size_t pos = formatted_number.find_last_not_of('0');

    if (pos != std::string::npos) 
    {
        if (formatted_number[pos] == '.') 
        {
            formatted_number.erase(pos);
        }
        
        else 
        {
            formatted_number.erase(pos + 1);
        }
    }
    
    return formatted_number;
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Number::added_to(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Value>>(other))
    {
        if (auto number = std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other)))
        {
            return { std::make_shared<Number>(this->value + std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other))->value, this->context), std::make_shared<NoError>() };
        }

        else
        {
            return { nullptr, Value::illegal_operation(other) };
        }
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Number::subbed_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Value>>(other))
    {
        if (auto number = std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other)))
        {
            return { std::make_shared<Number>(this->value - std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other))->value, this->context), std::make_shared<NoError>() };
        }

        else
        {
            return { nullptr, Value::illegal_operation(other) };
        }
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Number::multed_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Value>>(other))
    {
        if (auto number = std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other)))
        {
            return { std::make_shared<Number>(this->value * std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other))->value, this->context), std::make_shared<NoError>() };
        }

        else
        {
            return { nullptr, Value::illegal_operation(other) };
        }
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Number::dived_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Value>>(other))
    {
        if (auto number = std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other)))
        {
            return { EMPTY_NUMBER, std::make_shared<RTError> (
                std::get<std::shared_ptr<Value>>(other)->pos_start, std::get<std::shared_ptr<Value>>(other)->pos_start,
                "Division by zero",
                this->context
            )};

            return { std::make_shared<Number>(this->value / std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other))->value, this->context), std::make_shared<NoError>() };
        }

        else
        {
            return { nullptr, Value::illegal_operation(other) };
        }
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Number::powed_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Value>>(other))
    {
        if (auto number = std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other)))
        {
            return { std::make_shared<Number>(pow(this->value, std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other))->value), this->context), std::make_shared<NoError>() };
        }

        else
        {
            return { nullptr, Value::illegal_operation(other) };
        }
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Number::get_comparison_eq(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Value>>(other))
    {
        if (auto number = std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other)))
        {
            return { std::make_shared<Number>(this->value == std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other))->value, this->context), std::make_shared<NoError>() };
        }

        else
        {
            return { nullptr, Value::illegal_operation(other) };
        }
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Number::get_comparison_ne(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Value>>(other))
    {
        if (auto number = std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other)))
        {
            return { std::make_shared<Number>(this->value != std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other))->value, this->context), std::make_shared<NoError>() };
        }

        else
        {
            return { nullptr, Value::illegal_operation(other) };
        }
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Number::get_comparison_lt(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Value>>(other))
    {
        if (auto number = std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other)))
        {
            return { std::make_shared<Number>(this->value < std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other))->value, this->context), std::make_shared<NoError>() };
        }

        else
        {
            return { nullptr, Value::illegal_operation(other) };
        }
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Number::get_comparison_gt(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Value>>(other))
    {
        if (auto number = std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other)))
        {
            return { std::make_shared<Number>(this->value > std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other))->value, this->context), std::make_shared<NoError>() };
        }

        else
        {
            return { nullptr, Value::illegal_operation(other) };
        }
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Number::get_comparison_lte(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Value>>(other))
    {
        if (auto number = std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other)))
        {
            return { std::make_shared<Number>(this->value <= std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other))->value, this->context), std::make_shared<NoError>() };
        }

        else
        {
            return { nullptr, Value::illegal_operation(other) };
        }
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Number::get_comparison_gte(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Value>>(other))
    {
        if (auto number = std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other)))
        {
            return { std::make_shared<Number>(this->value >= std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other))->value, this->context), std::make_shared<NoError>() };
        }

        else
        {
            return { nullptr, Value::illegal_operation(other) };
        }
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Number::anded_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Value>>(other))
    {
        if (auto number = std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other)))
        {
            return { std::make_shared<Number>(this->value && std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other))->value, this->context), std::make_shared<NoError>() };
        }

        else
        {
            return { nullptr, Value::illegal_operation(other) };
        }
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Number::ored_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Value>>(other))
    {
        if (auto number = std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other)))
        {
            return { std::make_shared<Number>(this->value || std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other))->value, this->context), std::make_shared<NoError>() };
        }

        else
        {
            return { nullptr, Value::illegal_operation(other) };
        }
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> Number::notted_by()
{
    return { std::make_shared<Number>((this->value == 0) ? 1 : 0, this->context), std::make_shared<NoError>() };
}

std::shared_ptr<Value> Number::copy()
{
    return std::enable_shared_from_this<Value>::shared_from_this();
}

bool Number::is_true()
{
    return this->value != 0;
}

/* ---------------------------------------------------------------------------- */

String::String(std::string value, std::shared_ptr<Context> context, std::shared_ptr<Position> pos_start, std::shared_ptr<Position> pos_end)
{
    this->value = value;

    if (pos_start != nullptr) { this->pos_start = pos_start; }
    if (pos_end != nullptr) { this->pos_end = pos_end; }
    if (context != nullptr) { this->context = context; }
}

String::~String()
{

}

/**
 * Returns value as string
 * @return std::string
*/
std::string String::repr()
{
    return this->value;
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> String::added_to(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Value>>(other))
    {
        if (auto str = std::dynamic_pointer_cast<String>(std::get<std::shared_ptr<Value>>(other)))
        {
            return { std::make_shared<String>(this->value += std::dynamic_pointer_cast<String>(std::get<std::shared_ptr<Value>>(other))->value, this->context), std::make_shared<NoError>() };
        }

        else
        {
            return { nullptr, Value::illegal_operation(other) };
        }
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> String::multed_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Value>>(other))
    {
        if (auto number = std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other)))
        {
            return { std::make_shared<String>(this->multiply_string(this->value, std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other))->value), this->context), std::make_shared<NoError>() };
        }

        else
        {
            return { nullptr, Value::illegal_operation(other) };
        }
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

bool String::is_true()
{
    return (this->value.size() > 0);
}

std::shared_ptr<Value> String::copy()
{
    return std::enable_shared_from_this<Value>::shared_from_this();
}

std::string String::multiply_string(std::string str, double times)
{
    std::string ret = "";

    for (double i = 0; i < times; i++)
    {
        ret += str;
    }

    return ret;
}

/* ---------------------------------------------------------------------------- */

List::List(std::vector<std::shared_ptr<Value>> elements, std::shared_ptr<Context> context, std::shared_ptr<Position> pos_start, std::shared_ptr<Position> pos_end)
{
    this->elements = elements;

    if (pos_start != nullptr) { this->pos_start = pos_start; }
    if (pos_end != nullptr) { this->pos_end = pos_end; }
    if (context != nullptr) { this->context = context; }
}

List::~List()
{

}

/**
 * Returns value as string
 * @return std::string
*/
std::string List::repr()
{
    std::string result = "[";

    for (auto &&i : this->elements)
    {
        if (auto x = std::dynamic_pointer_cast<Number>(i))
        {
            result += x->repr() += std::string(", ");
        }
    }

    if (this->elements.size() > 1)
    {
        result.pop_back();
        result.pop_back();
    }

    result += "]";
    return result;
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> List::added_to(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Value>>(other))
    {
        std::shared_ptr<List> new_list = std::dynamic_pointer_cast<List>(this->copy());
        new_list->elements.push_back(std::get<0>(other));

        return { new_list, std::make_shared<NoError>() };
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> List::subbed_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Value>>(other))
    {
        if (auto number = std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other)))
        {
            std::shared_ptr<List> new_list = std::dynamic_pointer_cast<List>(this->copy());

            try
            {
                new_list->elements.erase(new_list->elements.begin() + static_cast<size_t>(std::dynamic_pointer_cast<Number>(std::get<0>(other))->value));
                return { new_list, std::make_shared<NoError>() };
            }

            catch (const std::out_of_range& e)
            {
                return { nullptr, std::make_shared<RTError> (
                    std::dynamic_pointer_cast<Number>(std::get<0>(other))->pos_start, std::dynamic_pointer_cast<Number>(std::get<0>(other))->pos_end,
                    "Element at this index could not be removed from list because index is out of bounds",
                    this->context
                )};
            }
        }

        else
        {
            return { nullptr, Value::illegal_operation(other) };
        }
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> List::multed_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    if (std::holds_alternative<std::shared_ptr<Value>>(other))
    {
        if (auto list = std::dynamic_pointer_cast<List>(std::get<std::shared_ptr<Value>>(other)))
        {
            std::shared_ptr<List> new_list = std::dynamic_pointer_cast<List>(this->copy());

            extend(new_list->elements, std::dynamic_pointer_cast<List>(std::get<0>(other))->elements);
            return { new_list, std::make_shared<NoError>() };
        }

        else
        {
            return { nullptr, Value::illegal_operation(other) };
        }
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> List::dived_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other)
{
    if (auto number = std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(other)))
    {
        std::shared_ptr<List> new_list = std::dynamic_pointer_cast<List>(this->copy());

        try
        {
            return { this->elements.at(static_cast<size_t>(std::dynamic_pointer_cast<Number>(std::get<0>(other))->value)), std::make_shared<NoError>() };
        }

        catch (const std::out_of_range& e)
        {
            return { nullptr, std::make_shared<RTError> (
                std::dynamic_pointer_cast<Number>(std::get<0>(other))->pos_start, std::dynamic_pointer_cast<Number>(std::get<0>(other))->pos_end,
                "Element at this index could not be retrieved from list because index is out of bound",
                this->context
            )};
        }
    }

    else
    {
        return { nullptr, Value::illegal_operation(other) };
    }

    throw ValueWrongType();
    return { nullptr, nullptr };
}

std::shared_ptr<Value> List::copy()
{
    return std::enable_shared_from_this<Value>::shared_from_this();
}

/* ---------------------------------------------------------------------------- */

BaseFunction::BaseFunction(std::string name, std::shared_ptr<Context> context, std::shared_ptr<Position> pos_start, std::shared_ptr<Position> pos_end) : Value(context, pos_start, pos_end)
{
    if (name != "") { this->name = name; } else { this->name = "<anonymous>"; }
}

BaseFunction::~BaseFunction()
{

}

std::shared_ptr<Context> BaseFunction::generate_new_context()
{
    std::shared_ptr<Context> new_context = std::make_shared<Context>(this->name, this->context, this->pos_start);
    new_context->symbol_table = new_context->parent->symbol_table;

    return new_context;
}

std::shared_ptr<RTResult> BaseFunction::check_args(std::vector<std::string> arg_names, std::vector<std::shared_ptr<Value>> args)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();

    if (args.size() > arg_names.size())
    {
        return res->failure(std::make_shared<RTError> (
            this->pos_start, this->pos_end,
            (std::to_string(args.size() - arg_names.size()) += std::string("too many args passed into ") += this->name),
            this->context
        ));
    }

    if (args.size() < arg_names.size())
    {
        return res->failure(std::make_shared<RTError> (
            this->pos_start, this->pos_end,
            (std::to_string(arg_names.size() - args.size()) += std::string("too few args passed into ") += this->name),
            this->context
        ));
    }

    return res->success(nullptr);
}

void BaseFunction::populate_args(std::vector<std::string> arg_names, std::vector<std::shared_ptr<Value>> args, std::shared_ptr<Context> exec_ctx)
{
    for (size_t i = 0; i < arg_names.size(); i++)
    {
        std::string arg_name = arg_names.at(i);
        std::shared_ptr<Value> arg_value = args.at(i);
        
        arg_value->context = exec_ctx;
        exec_ctx->symbol_table->set(arg_name, arg_value);
    }
}

std::shared_ptr<RTResult> BaseFunction::check_and_populate_args(std::vector<std::string> arg_names, std::vector<std::shared_ptr<Value>> args, std::shared_ptr<Context> exec_ctx)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();

    res->register_result(this->check_args(arg_names, args));
    if (res->should_return()) { return res; }

    this->populate_args(arg_names, args, exec_ctx);
    return res->success(nullptr);
}

/* ---------------------------------------------------------------------------- */

Function::Function(std::string name, ALL_VARIANT body_node, std::vector<std::string> arg_names, bool should_auto_return, std::shared_ptr<Context> context, std::shared_ptr<Position> pos_start, std::shared_ptr<Position> pos_end) : BaseFunction(name, context, pos_start, pos_end)
{
    if (name != "") { this->name = name; } else { this->name = "<anonymous>"; }

    this->body_node = body_node;
    this->arg_names = arg_names;
    this->should_auto_return = should_auto_return;
}

Function::~Function()
{

}

/**
 * Returns value as string
 * @return std::string
*/
std::string Function::repr()
{
    return (std::string("<function ") += this->name += std::string(">"));
}

std::shared_ptr<RTResult> Function::execute(std::vector<std::shared_ptr<Value>> args)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();
    Interpreter interpreter = Interpreter();

    std::shared_ptr<Context> exec_ctx = this->generate_new_context();

    res->register_result(this->check_and_populate_args(this->arg_names, args, exec_ctx));
    if (res->should_return()) { return res; }

    std::shared_ptr<Value> value = res->register_result(interpreter.visit(this->body_node, exec_ctx));
    if (res->should_return() && res->func_return_value == nullptr) { return res; }

    std::shared_ptr<Value> ret_value = this->should_auto_return ? value : (res->func_return_value != nullptr ? res->func_return_value : std::make_shared<Number>(0));
    return res->success(ret_value);
}

std::shared_ptr<Value> Function::copy()
{
    std::shared_ptr<Function> copy = std::make_shared<Function>(this->name, this->body_node, this->arg_names, this->should_auto_return);

    copy->context = this->context;
    copy->pos_start = this->pos_start;
    copy->pos_end = this->pos_end;

    return copy;
}

/* ---------------------------------------------------------------------------- */

BuiltInFunction::BuiltInFunction(std::string name, std::shared_ptr<Context> context, std::shared_ptr<Position> pos_start, std::shared_ptr<Position> pos_end) : BaseFunction(name, context, pos_start, pos_end) /* we could just pass name variable as CodePulse */
{
    
}

BuiltInFunction::~BuiltInFunction()
{

}

/**
 * Returns value as string
 * @return std::string
*/
std::string BuiltInFunction::repr()
{
    return (std::string("<built-in function ") += this->name += std::string(">"));
}

std::shared_ptr<RTResult> BuiltInFunction::execute(std::vector<std::shared_ptr<Value>> args)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();
    std::shared_ptr<Context> exec_ctx = this->generate_new_context();

    std::map<const std::string, std::function<std::shared_ptr<RTResult>(std::shared_ptr<Context>)>> functions = 
    {
        {"clear", [this](std::shared_ptr<Context> context) { return this->execute_clear(context); }},
        {"print", [this](std::shared_ptr<Context> context) { return this->execute_print(context); }},
        {"write", [this](std::shared_ptr<Context> context) { return this->execute_write(context); }},
        {"len", [this](std::shared_ptr<Context> context) { return this->execute_len(context); }},
        {"run", [this](std::shared_ptr<Context> context) { return this->execute_run(context); }},
    };

    if (this->name == "clear")
    {
        /* no arguments needed for clear function */
    }

    else if (this->name == "print")
    {
        this->arg_names.push_back("value");
    }

    else if (this->name == "write")
    {
        this->arg_names.push_back("value");
    }

    else if (this->name == "len")
    {
        this->arg_names.push_back("list");
    }

    else if (this->name == "run")
    {
        this->arg_names.push_back("fn");
    }

    else
    {
        throw NoBuiltInFunction();
        return nullptr;
    }

    res->register_result(this->check_and_populate_args(this->arg_names, args, exec_ctx));
    if (res->error->error_name != "") { return res; }

    std::shared_ptr<Value> return_value;
    auto function = functions.find(this->name);

    if (function != functions.end())
    {
        return_value = res->register_result(functions[this->name](exec_ctx));
        if (res->error->error_name != "") { return res; }
    }

    else
    {
        throw NoBuiltInFunction();
        return nullptr;
    }

    return res->success(return_value);
}

std::shared_ptr<Value> BuiltInFunction::copy()
{
    std::shared_ptr<BuiltInFunction> copy = std::make_shared<BuiltInFunction>(this->name, this->context, this->pos_start, this->pos_end);
    return copy;
}

/* ------------------------------------ */

std::shared_ptr<RTResult> BuiltInFunction::execute_clear(std::shared_ptr<Context> exec_ctx)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();

    #if defined _WIN32 || defined _WIN64
        system("cls");
    #elif __APPLE__ || __MACH__ || __linux__ || __FreeBSD__
        system("clear");
    #endif

    return res->success(std::make_shared<Number>(0)); /* Number.null - CodePulse's code */
};

std::shared_ptr<RTResult> BuiltInFunction::execute_print(std::shared_ptr<Context> exec_ctx)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();

    auto result = exec_ctx->symbol_table->get_value("value");
    if (!std::holds_alternative<std::nullptr_t>(result))
    {
        auto final_result = std::get<std::shared_ptr<Value>>(result);

        if (std::dynamic_pointer_cast<Number>(final_result))
        {
            std::cout << std::dynamic_pointer_cast<Number>(final_result)->repr();
        }

        else if (std::dynamic_pointer_cast<String>(final_result))
        {
            std::cout << std::dynamic_pointer_cast<String>(final_result)->repr();
        }

        else if (std::dynamic_pointer_cast<List>(final_result))
        {
            std::cout << std::dynamic_pointer_cast<List>(final_result)->repr();
        }

        else if (std::dynamic_pointer_cast<Function>(final_result))
        {
            std::cout << std::dynamic_pointer_cast<Function>(final_result)->repr();
        }

        else if (std::dynamic_pointer_cast<BuiltInFunction>(final_result))
        {
            std::cout << std::dynamic_pointer_cast<BuiltInFunction>(final_result)->repr();
        }

        else
        {
            std::cout << std::get<std::shared_ptr<Value>>(result)->repr();
        }

        std::cout << std::endl;
    }

    return res->success(std::make_shared<Number>(0)); /* Number.null - CodePulse's code */
};

std::shared_ptr<RTResult> BuiltInFunction::execute_write(std::shared_ptr<Context> exec_ctx)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();

    auto result = exec_ctx->symbol_table->get_value("value");
    if (!std::holds_alternative<std::nullptr_t>(result))
    {
        auto final_result = std::get<std::shared_ptr<Value>>(result);

        if (std::dynamic_pointer_cast<Number>(final_result))
        {
            std::cout << std::dynamic_pointer_cast<Number>(final_result)->repr();
        }

        else if (std::dynamic_pointer_cast<String>(final_result))
        {
            std::cout << std::dynamic_pointer_cast<String>(final_result)->repr();
        }

        else if (std::dynamic_pointer_cast<List>(final_result))
        {
            std::cout << std::dynamic_pointer_cast<List>(final_result)->repr();
        }

        else if (std::dynamic_pointer_cast<Function>(final_result))
        {
            std::cout << std::dynamic_pointer_cast<Function>(final_result)->repr();
        }

        else if (std::dynamic_pointer_cast<BuiltInFunction>(final_result))
        {
            std::cout << std::dynamic_pointer_cast<BuiltInFunction>(final_result)->repr();
        }

        else
        {
            std::cout << std::get<std::shared_ptr<Value>>(result)->repr();
        }
    }

    return res->success(std::make_shared<Number>(0)); /* Number.null - CodePulse's code */
};

std::shared_ptr<RTResult> BuiltInFunction::execute_len(std::shared_ptr<Context> exec_ctx)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();

    auto result = exec_ctx->symbol_table->get_value("list");
    if (!std::holds_alternative<std::nullptr_t>(result))
    {
        auto final_result = std::get<std::shared_ptr<Value>>(result);

        if (std::dynamic_pointer_cast<List>(final_result))
        {
            return res->success(std::make_shared<Number>(std::dynamic_pointer_cast<List>(final_result)->elements.size()));
        }

        else
        {
            return res->failure(std::make_shared<RTError> (
                this->pos_start, this->pos_end,
                "Argument must be list",
                exec_ctx
            ));
        }
    }

    return res->success(std::make_shared<Number>(0));
};

std::shared_ptr<RTResult> BuiltInFunction::execute_run(std::shared_ptr<Context> exec_ctx)
{
    std::shared_ptr<RTResult> res = std::make_shared<RTResult>();

    auto result = exec_ctx->symbol_table->get_value("fn");
    if (!std::holds_alternative<std::nullptr_t>(result))
    {
        auto final_result = std::get<std::shared_ptr<Value>>(result);

        if (std::dynamic_pointer_cast<String>(final_result))
        {
            std::fstream my_file;
            std::string file = std::dynamic_pointer_cast<String>(final_result)->value;

            std::string script = "";
            
            my_file.open(file, std::ios::in);
            if (!my_file)
            {
                std::string details = std::string("No such file ") += file += std::string("!");
                return res->failure(std::make_shared<RTError> (
                    this->pos_start, this->pos_end,
                    details,
                    exec_ctx
                ));
            }

            else
            {
                std::string str;
                while (getline(my_file, str))
                {
                    script += (str += std::string("\n"));
                }

                script += std::string("\n");
            }

            std::variant<std::shared_ptr<Value>, std::shared_ptr<Error>> result = run_2(file, script);

            if (std::holds_alternative<std::shared_ptr<Error>>(result))
            {
                std::string details = std::string("Failed to finish executing script ") += file += std::string("\n") += std::get<std::shared_ptr<Error>>(result)->as_string();
                return res->failure(std::make_shared<RTError> (
                    this->pos_start, this->pos_end,
                    details,
                    exec_ctx
                ));
            }
        }

        else
        {
            return res->failure(std::make_shared<RTError> (
                this->pos_start, this->pos_end,
                "Argument must be string",
                exec_ctx
            ));
        }
    }

    return res->success(std::make_shared<Number>(0));
};
