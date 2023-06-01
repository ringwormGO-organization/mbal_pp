/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include <iostream>
#include <string>
#include <tuple>
#include <variant>

#include "empty.hpp"

#include "src/Error.hpp"
#include "src/Interpreter.hpp"
#include "src/Lexer.hpp"
#include "src/Nodes.hpp"
#include "src/Parser.hpp"
#include "src/Token.hpp"

std::tuple<Parser, ParseResult, std::variant<NumberNode*, BinOpNode*, UnaryOpNode*>, Error> run(std::string fn, std::string text)
{
    /* Generate tokens */
    Lexer lexer(fn, text);
    std::tuple<std::vector<Token>, Error> result = lexer.make_tokens();

    if (std::get<1>(result).error_name.size() > 0)
    {
        return {EMPTY_PARSER, EMPTY_PARSE_RESULT, EMPTY_NODE , std::get<1>(result) };
    }

    /* Generate AST */
    Parser parser(std::get<0>(result));
    ParseResult ast = parser.parse();

    if (ast.error.error_name != "")
    {
        return { EMPTY_PARSER, EMPTY_PARSE_RESULT, EMPTY_NODE, ast.error };
    }

    /* Run program */
    Interpreter interpreter;
    interpreter.visit(ast.node, Context());

    return { parser, ast, ast.node, ast.error };
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

        std::tuple<Parser, ParseResult, std::variant<NumberNode*, BinOpNode*, UnaryOpNode*>, Error> result = run("<stdin>", input);

        if (std::get<Error>(result).error_name.size() > 0)
        {
            std::cout << std::get<Error>(result).as_string() << '\n';
        }

        else
        {
            for (size_t i = 0; i < (std::get<Parser>(result).tokens.size() - 1); i++)
            {
                std::cout << std::get<Parser>(result).tokens.at(i).repr();
            }

            /*for (auto &&i : std::get<Parser>(result).tokens)
            {
                std::cout << i.repr();
            }*/

            std::cout << '\n';

            auto delete_node = [](auto ptr) {
                delete ptr;
            };
            std::visit(delete_node, std::get<1>(result).node);
        }
    }

    return 0;
}
