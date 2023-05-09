/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include <iostream>
#include <string>
#include <tuple>
#include <variant>

#include "src/Error.hpp"
#include "src/Lexer.hpp"
#include "src/Nodes.hpp"
#include "src/Parser.hpp"
#include "src/Token.hpp"

static const Token EMPTY_TOKEN = Token(TT::NUL);
static const std::vector<Token> EMPTY_VECTOR;

static const std::variant<NumberNode, BinOpNode> EMPTY_NODE = NumberNode(EMPTY_TOKEN);
static const Parser EMPTY_PARSER = Parser(EMPTY_VECTOR);

std::tuple<std::variant<NumberNode, BinOpNode>, Parser, Error> run(std::string fn, std::string text)
{
    /* Generate tokens */
    Lexer lexer(fn, text);
    std::tuple<std::vector<Token>, Error> result = lexer.make_tokens();

    if (std::get<1>(result).error_name.size() > 0)
    {
        return { EMPTY_NODE, EMPTY_PARSER, std::get<1>(result) };
    }

    /* Generate AST */
    Parser parser(std::get<0>(result));
    auto ast = parser.parse();

    return { ast, parser, NoError() };
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

        std::tuple<std::variant<NumberNode, BinOpNode>, Parser, Error> result = run("<stdin>", input);

        if (std::get<Error>(result).error_name.size() > 0)
        {
            std::cout << std::get<2>(result).as_string() << '\n';
        }

        else
        {
            for (auto &&i : std::get<Parser>(result).tokens)
            {
                std::cout << i.repr();
            }

            std::cout << '\n';
        }
    }

    return 0;
}
