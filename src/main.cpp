/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include <iostream>
#include <string>

#include "src/Token.hpp"
#include "src/Lexer.hpp"

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

        Lexer lexer("<stdin>", input);
        auto result = lexer.make_tokens();

        if (std::get<1>(result).error_name.size() > 0)
        {
            std::cout << std::get<1>(result).as_string() << '\n';
        }

        else
        {
            for (auto &&i : std::get<0>(result))
            {
                std::cout << i.repr();
            }

            std::cout << '\n';
        }
    }

    return 0;
}
