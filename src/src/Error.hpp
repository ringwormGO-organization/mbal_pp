/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <algorithm>
#include <exception>
#include <iostream>
#include <memory>
#include <string>

#include "Context.hpp"
#include "Position.hpp"

static const Position EMPTY_POSITION = Position(-1, 0, -1, "", "");

std::string string_with_arrows(std::string text, Position pos_start, Position pos_end);

class Error
{
    public:
        Error(Position pos_start, Position pos_end, std::string error_name, std::string details);
        virtual ~Error() {};

        virtual std::string as_string();
        std::string error_name = "";
    protected:
        Position pos_start;
        Position pos_end;
        std::string details = "";
};

class IllegalCharError : public Error
{
    public:
        IllegalCharError(Position pos_start, Position pos_end, std::string details) : Error(pos_start, pos_end, "Illegal Character", details) {};
};

class InvalidSyntaxError : public Error
{
    public:
        InvalidSyntaxError(Position pos_start, Position pos_end, std::string details) : Error(pos_start, pos_end, "Invalid Syntax", details) {};
};

class RTError : public Error
{
    public:
        RTError(Position pos_start, Position pos_end, std::string details, std::shared_ptr<Context> context) : Error(pos_start, pos_end, "Runtime Error", details) 
        {
            this->context = context;
        };

        std::string as_string() override
        {
            std::string result = this->generate_traceback();

            result += this->error_name += this->details;
            result += std::string("\n\n") += string_with_arrows(this->pos_start.ftxt, this->pos_start, this->pos_end);

            return result;
        };

    private:
        std::string generate_traceback()
        {
            std::string result = "";

            Position pos = this->pos_start;
            std::shared_ptr<Context> ctx = this->context;

            while (ctx) 
            {
                result = std::string("File ") += pos.fn += std::string(", line ") += std::to_string(pos.ln + 1) += std::string(", in ") += ctx->display_name += std::string("\n") += result;
                pos = ctx->parent_entry_pos;
                ctx = ctx->parent;
            }

            return (std::string("Traceback (most recent call last): \n") += result);
        };

        std::shared_ptr<Context> context;
};

class NoError : public Error
{
    public:
        NoError() : Error(EMPTY_POSITION, EMPTY_POSITION, "", "") {};
};

/* ---------------------------------------------------------------------------- */

class InterpreterWrongType : public std::exception
{
    public:
        InterpreterWrongType() noexcept = default;

        const char* what() const noexcept override { return "[InterpreterWrongType] Wrong type in `node` variable"; }
};
