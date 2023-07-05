/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include <iostream>
#include <memory>
#include <string>
#include <variant>

#include "src/Error.hpp"
#include "src/Interpreter.hpp"
#include "src/Lexer.hpp"
#include "src/Nodes.hpp"
#include "src/Parser.hpp"
#include "src/SymbolTable.hpp"
#include "src/Token.hpp"

/**
 * Global symbols
*/
std::shared_ptr<SymbolTable> global_symbol_table = std::make_shared<SymbolTable>();

/**
 * Calls lexer, parser and interpreter
 * @param fn name of function
 * @param text input
 * @return result (Number class) or error (Error class)
*/
std::variant<std::shared_ptr<Value>, std::shared_ptr<Error>> run(std::string fn, std::string text)
{
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
    std::shared_ptr<RTResult> interpreter_result = interpreter.visit(ast.get()->node, context);

    if (interpreter_result->error->error_name != "")
    {
        return interpreter_result->error;
    }

    return interpreter_result->value;
}

/**
 * Entry point
 * @return zero
*/
int main()
{
    std::string input;

    while (true)
    {
        std::cout << "mbal++ > ";
        getline(std::cin, input);

        if (input == "exit")
        {
            exit(0);
        }

        global_symbol_table->set(std::string("NULL"), std::make_shared<Number>(0));
        global_symbol_table->set(std::string("TRUE"), std::make_shared<Number>(1));
        global_symbol_table->set(std::string("FALSE"), std::make_shared<Number>(0));

        std::variant<std::shared_ptr<Value>, std::shared_ptr<Error>> result = run("<stdin>", input);

        if (std::holds_alternative<std::shared_ptr<Error>>(result))
        {
            std::cout << std::get<1>(result)->as_string() << '\n';
        }

        else
        {
            if (auto temporary = std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(result)))
            {
                std::cout << std::dynamic_pointer_cast<Number>(std::get<std::shared_ptr<Value>>(result))->repr();
            }

            else if (auto temporary = std::dynamic_pointer_cast<List>(std::get<std::shared_ptr<Value>>(result)))
            {
                std::cout << std::dynamic_pointer_cast<List>(std::get<std::shared_ptr<Value>>(result))->repr();
            }

            else if (auto temporary = std::dynamic_pointer_cast<String>(std::get<std::shared_ptr<Value>>(result)))
            {
                std::cout << std::dynamic_pointer_cast<String>(std::get<std::shared_ptr<Value>>(result))->repr();
            }

            else if (auto temporary = std::dynamic_pointer_cast<Function>(std::get<std::shared_ptr<Value>>(result)))
            {
                std::cout << std::dynamic_pointer_cast<Function>(std::get<std::shared_ptr<Value>>(result))->repr();
            }

            else
            {
                std::cout << std::get<std::shared_ptr<Value>>(result)->repr();
            }
            
            std::cout << '\n';
        }
    }

    return 0;
}
