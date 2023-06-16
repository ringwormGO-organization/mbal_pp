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

Lexer::Lexer(std::string fn, std::string text)
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
    if (this->first_run)
    {
        first_run = false;
    }

    else
    {
        this->pos->advance(this->current_char);
    }

    if (this->pos != nullptr)
    {
        if (this->pos->idx < this->text.size())
        {
            this->current_char = text.at(this->pos->idx);
        }

        else
        {
            this->current_char = '\0';
        }
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

        else if (this->current_char == '!')
        {
            auto [tok, error] = this->make_not_equals();

            if (error->error_name != "") { return std::make_tuple(empty_vector, error); }
            tokens.push_back(tok);
        }

        else if (this->current_char == '=')
        {
            tokens.push_back(this->make_equals());
        }

        else if (this->current_char == '<')
        {
            tokens.push_back(this->make_less_than());
        }

        else if (this->current_char == '>')
        {
            tokens.push_back(this->make_greater_than());
        }

        else
        {
            std::shared_ptr<Position> pos_start = this->pos->copy();

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
    std::shared_ptr<Position> pos_start = this->pos->copy();

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
    std::shared_ptr<Position> pos_start = this->pos->copy();

    while (this->current_char != '\0' && (std::isdigit(this->current_char) || std::isalpha(this->current_char) || this->current_char == '_')) 
    {
        id_str += std::string(1, this->current_char);
        this->advance();
    }

    TT tok_type;
    (contains(KEYWORDS, id_str)) ? tok_type = TT::KEYWORD : tok_type = TT::IDENTIFIER;

    return Token(tok_type, pos_start, id_str, this->pos);
}

std::tuple<Token, std::shared_ptr<Error>> Lexer::make_not_equals()
{
    std::shared_ptr<Position> pos_start = this->pos->copy();
    this->advance();

    if (this->current_char == '=')
    {
        this->advance();
        return { Token(TT::NE, pos_start, "", this->pos), std::make_shared<NoError>() };
    }

    this->advance();
    return { Token(TT::NUL), std::make_shared<ExpectedCharError>(pos_start, this->pos, "'=' after ('!')") };
}

Token Lexer::make_equals()
{
    TT tok_type = TT::EQ;
    std::shared_ptr<Position> pos_start = this->pos->copy();
    this->advance();

    if (this->current_char == '=')
    {
        this->advance();
        tok_type = TT::EE;
    }

    return Token(tok_type, pos_start, "", this->pos);
}

Token Lexer::make_less_than()
{
    TT tok_type = TT::LT;
    std::shared_ptr<Position> pos_start = this->pos->copy();
    this->advance();

    if (this->current_char == '=')
    {
        this->advance();
        tok_type = TT::LTE;
    }

    return Token(tok_type, pos_start, "", this->pos);
}

Token Lexer::make_greater_than()
{
    TT tok_type = TT::GT;
    std::shared_ptr<Position> pos_start = this->pos->copy();
    this->advance();

    if (this->current_char == '=')
    {
        this->advance();
        tok_type = TT::GTE;
    }

    return Token(tok_type, pos_start, "", this->pos);
}
