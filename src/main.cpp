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
#include "src/Token.hpp"

std::variant<std::shared_ptr<Number>, std::shared_ptr<Error>> run(std::string fn, std::string text)
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

    std::shared_ptr<RTResult> interpreter_result = interpreter.visit(ast.get()->node, context);

    if (interpreter_result->error->error_name != "")
    {
        return interpreter_result->error;
    }

    return interpreter_result->value;
}

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

        std::variant<std::shared_ptr<Number>, std::shared_ptr<Error>> result = run("<stdin>", input);

        if (std::holds_alternative<std::shared_ptr<Error>>(result))
        {
            std::cout << std::get<1>(result)->as_string() << '\n';
        }

        else
        {
            std::cout << std::get<0>(result)->repr();
            std::cout << '\n';
        }
    }

    return 0;
}
