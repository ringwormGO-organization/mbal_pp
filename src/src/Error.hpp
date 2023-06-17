/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
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

std::string string_with_arrows(std::string text, std::shared_ptr<Position> pos_start, std::shared_ptr<Position> pos_end);

/**
 * Class handling errors
 * @param pos_start starting position
 * @param pos_end ending position
 * @param error_name name of error (if empty, there is no error)
 * @param details details of error
*/
class Error
{
    public:
        Error(std::shared_ptr<Position> pos_start, std::shared_ptr<Position> pos_end, std::string error_name, std::string details);
        virtual ~Error() {};

        /**
         * Returns all error details as std::string
         * @return std::string
        */
        virtual std::string as_string();

        std::string error_name = "";
        std::shared_ptr<Position> pos_start = std::make_shared<Position>(0, 0, 0, "", "");
        std::shared_ptr<Position> pos_end = std::make_shared<Position>(0, 0, 0, "", "");
        std::string details = "";
};

/**
 * Class handling illegal character error
 * @param pos_start starting position
 * @param pos_end ending position
 * @param details details of error
*/
class IllegalCharError : public Error
{
    public:
        IllegalCharError(std::shared_ptr<Position> pos_start, std::shared_ptr<Position> pos_end, std::string details) : Error(pos_start, pos_end, "Illegal Character", details) {};
};

/**
 * Class handling expected character error
 * @param pos_start starting position
 * @param pos_end ending position
 * @param details details of error
*/
class ExpectedCharError : public Error
{
    public:
        ExpectedCharError(std::shared_ptr<Position> pos_start, std::shared_ptr<Position> pos_end, std::string details) : Error(pos_start, pos_end, "Expected Character", details) {};
};

/**
 * Class handling invalid syntax error
 * @param pos_start starting position
 * @param pos_end ending position
 * @param details details of error
*/
class InvalidSyntaxError : public Error
{
    public:
        InvalidSyntaxError(std::shared_ptr<Position> pos_start, std::shared_ptr<Position> pos_end, std::string details) : Error(pos_start, pos_end, "Invalid Syntax", details) {};
};

/**
 * Class handling runtime result error
 * @param pos_start starting position
 * @param pos_end ending position
 * @param details details of error
 * @param context context
*/
class RTError : public Error
{
    public:
        RTError(std::shared_ptr<Position> pos_start, std::shared_ptr<Position> pos_end, std::string details, std::shared_ptr<Context> context) : Error(pos_start, pos_end, "Runtime Error", details) 
        {
            this->context = context;
        };

        std::string as_string() override
        {
            std::string result = this->generate_traceback();

            result += this->error_name += std::string(": ") += this->details;
            result += std::string("\n\n") += string_with_arrows(this->pos_start->ftxt, this->pos_start, this->pos_end);

            return result;
        };

    public:
        std::shared_ptr<Context> context;

    private:

        /**
         * Generates traceback
         * @return std::string
        */
        std::string generate_traceback()
        {
            std::string result = "";

            std::shared_ptr<Position> pos = this->pos_start;
            std::shared_ptr<Context> ctx = this->context;

            while (ctx) 
            {
                result = std::string("File ") += pos->fn += std::string(", line ") += std::to_string(pos->ln + 1) += std::string(", in ") += ctx->display_name += std::string("\n") += result;
                pos = ctx->parent_entry_pos;
                ctx = ctx->parent;
            }

            return (std::string("Traceback (most recent call last): \n") += result);
        };

};

/**
 * Placeholder when there is no error
*/
class NoError : public Error
{
    public:
        NoError() : Error(std::make_shared<Position>(0, 0, 0, "", ""), std::make_shared<Position>(0, 0, 0, "", ""), "", "") {};
        std::string as_string() override { return ""; }
};

/* ---------------------------------------------------------------------------- */

/**
 * Class handling wrong type in `node` variable which is std::variant
*/
class InterpreterWrongType : public std::exception
{
    public:
        InterpreterWrongType() noexcept = default;

        const char* what() const noexcept override { return "[InterpreterWrongType] Wrong type in `node` variable"; }
};

/**
 * Class handling wrong type in `other` variable which is std::variant
*/
class ValueWrongType : public std::exception
{
    public:
        ValueWrongType() noexcept = default;

        const char* what() const noexcept override { return "[ValueWrongType] Wrong type in `other` variable"; }
};

/**
 * Class handling empty `node` variable due to incorrect initialization of variable
*/
class NodeEmpty : public std::exception
{
    public:
        NodeEmpty() noexcept = default;

        const char* what() const noexcept override { return "[NodeEmpty] `node` variable is empty\n\t   because of incorrect initialization of variable"; }
};
