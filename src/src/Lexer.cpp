/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Lexer.hpp"

Lexer::Lexer(std::string fn, std::string text) : pos(-1, 0, -1, fn, text)
{
    this->fn = fn;
    this->text = text;

    this->advance();
}

Lexer::~Lexer()
{

}

void Lexer::advance()
{
    this->pos.advance(this->current_char);
    
    if (this->pos.idx < this->text.size())
    {
        this->current_char = text.at(this->pos.idx);
    }

    else
    {
        this->current_char = '\0';
    }
}

std::tuple<std::vector<Token>, Error> Lexer::make_tokens()
{
    std::vector<Token> tokens;
    std::vector<Token> empty_vector;

    while (this->current_char != '\0')
    {
        if (this->current_char == ' ' || this->current_char == '\t')
        {
            this->advance();
        }

        else if (isdigit((int)this->current_char))
        {
            tokens.push_back(this->make_number());
        }

        else if (this->current_char == '+')
        {
            tokens.push_back(Token(TT::PLUS));
            this->advance();
        }

        else if (this->current_char == '-')
        {
            tokens.push_back(Token(TT::MINUS));
            this->advance();
        }

        else if (this->current_char == '*')
        {
            tokens.push_back(Token(TT::MUL));
            this->advance();
        }

        else if (this->current_char == '/')
        {
            tokens.push_back(Token(TT::DIV));
            this->advance();
        }

        else if (this->current_char == '-')
        {
            tokens.push_back(Token(TT::MINUS));
            this->advance();
        }

        else if (this->current_char == '(')
        {
            tokens.push_back(Token(TT::LPAREN));
            this->advance();
        }

        else if (this->current_char == ')')
        {
            tokens.push_back(Token(TT::RPAREN));
            this->advance();
        }

        else
        {
            Position pos_start = this->pos.copy();

            char ch = this->current_char;
            this->advance();

            return std::make_tuple(empty_vector, IllegalCharError(pos_start, this->pos, (std::string("'") += std::to_string(ch)) += "'"));
        }
    }

    return std::make_tuple(tokens, NoError());
}

Token Lexer::make_number()
{
    std::string num_str = "";
    size_t dot_count = 0;

    while (this->current_char != '\0' && ((DIGITS += ".").find(this->current_char) != std::string::npos))
    {
        if (this->current_char == '.')
        {
            if (dot_count == 1)
            {
                break;
            }

            dot_count += 1;
            num_str += ".";
        }

        else
        {
            num_str += this->current_char;
        }

        this->advance();
    }

    if (dot_count == 0)
    {
        return Token(TT::INT, num_str);
    }

    else
    {
        return Token(TT::FLOAT, num_str);
    }
}
