/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Lexer.hpp"

template<typename C, typename T>
bool contains(C&& c, T e) 
{ 
    return std::find(std::begin(c), std::end(c), e) != std::end(c);
}

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
    
    if (this->pos.idx < static_cast<signed long>(this->text.size()))
    {
        this->current_char = text.at(this->pos.idx);
    }

    else
    {
        this->current_char = '\0';
    }
}

std::tuple<std::vector<Token>, std::shared_ptr<Error>> Lexer::make_tokens()
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

        else if (isalpha((int)this->current_char))
        {
            tokens.push_back(this->make_identifier());
        }

        else if (this->current_char == '+')
        {
            tokens.push_back(Token(TT::PLUS, this->pos));
            this->advance();
        }

        else if (this->current_char == '-')
        {
            tokens.push_back(Token(TT::MINUS, this->pos));
            this->advance();
        }

        else if (this->current_char == '*')
        {
            tokens.push_back(Token(TT::MUL, this->pos));
            this->advance();
        }

        else if (this->current_char == '/')
        {
            tokens.push_back(Token(TT::DIV, this->pos));
            this->advance();
        }

        else if (this->current_char == '^')
        {
            tokens.push_back(Token(TT::POW, this->pos));
            this->advance();
        }

        else if (this->current_char == '=')
        {
            tokens.push_back(Token(TT::EQ, this->pos));
            this->advance();
        }

        else if (this->current_char == '(')
        {
            tokens.push_back(Token(TT::LPAREN, this->pos));
            this->advance();
        }

        else if (this->current_char == ')')
        {
            tokens.push_back(Token(TT::RPAREN, this->pos));
            this->advance();
        }

        else
        {
            Position pos_start = this->pos.copy();

            char ch = this->current_char;
            this->advance();

            return std::make_tuple(empty_vector, std::make_shared<IllegalCharError>(pos_start, this->pos, (std::string("'") += std::to_string(ch)) += "'"));
        }
    }

    tokens.push_back(Token(TT::END_OF_FILE, this->pos));
    return std::make_tuple(tokens, std::make_shared<NoError>());
}

Token Lexer::make_number()
{
    std::string num_str = "";
    size_t dot_count = 0;
    Position pos_start = this->pos.copy();

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
        return Token(TT::INT, pos_start, num_str, this->pos);
    }

    else
    {
        return Token(TT::FLOAT, pos_start, num_str, this->pos);
    }
}

Token Lexer::make_identifier()
{
    std::string id_str = "";
    Position pos_start = this->pos.copy();

    while (this->current_char != '\0' && (isdigit((int)this->current_char) || isdigit((int)this->current_char) || this->current_char == '_'))
    {
        id_str += std::string(1, this->current_char);
        this->advance();
    }

    TT tok_type;
    (contains(KEYWORDS, id_str)) ? tok_type = TT::KEYWORD : tok_type = TT::IDENTIFIER;

    return Token(tok_type, pos_start, id_str, this->pos);
}
