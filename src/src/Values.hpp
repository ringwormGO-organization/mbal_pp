/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <math.h>
#include <memory>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "define.hpp"

class NumberNode;
class StringNode;
class ListNode;
class VarAccessNode;
class VarAssignNode;
class BinOpNode;
class UnaryOpNode;
class IfNode;
class ForNode;
class WhileNode;
class DoNode;
class FuncDefNode;
class CallNode;
class ReturnNode;
class ContinueNode;
class BreakNode;

class Context;
class Error;
class Position;
class RTResult;
class Token;

#define UNUSED(x) (void)(x)

class Value : public virtual std::enable_shared_from_this<Value>
{
    public:
        Value(std::shared_ptr<Context> context=nullptr, std::shared_ptr<Position> pos_start=nullptr, std::shared_ptr<Position> pos_end=nullptr);
        virtual ~Value();

    public:
        /**
         * Returns value as string
         * @return std::string
        */
        virtual std::string repr();

        virtual std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> added_to(std::variant<std::shared_ptr<Value>, std::nullptr_t> other);
        virtual std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> subbed_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other);
        virtual std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> multed_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other);
        virtual std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> dived_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other);
        virtual std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> powed_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other);

        virtual std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> get_comparison_eq(std::variant<std::shared_ptr<Value>, std::nullptr_t> other);
        virtual std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> get_comparison_ne(std::variant<std::shared_ptr<Value>, std::nullptr_t> other);
        virtual std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> get_comparison_lt(std::variant<std::shared_ptr<Value>, std::nullptr_t> other);
        virtual std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> get_comparison_gt(std::variant<std::shared_ptr<Value>, std::nullptr_t> other);
        virtual std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> get_comparison_lte(std::variant<std::shared_ptr<Value>, std::nullptr_t> other);
        virtual std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> get_comparison_gte(std::variant<std::shared_ptr<Value>, std::nullptr_t> other);

        virtual std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> anded_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other);
        virtual std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> ored_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other);
        virtual std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> notted_by();

        virtual std::shared_ptr<RTResult> execute(std::vector<std::shared_ptr<Value>> args);

        virtual std::shared_ptr<Value> copy();
        virtual bool is_true();

    protected:
        std::shared_ptr<Error> illegal_operation(std::variant<std::shared_ptr<Value>, std::nullptr_t> other=nullptr);

    public:
        std::shared_ptr<Position> pos_start = std::make_shared<Position>(0, 0, 0, "", "");
        std::shared_ptr<Position> pos_end = std::make_shared<Position>(0, 0, 0, "", "");

        std::shared_ptr<Context> context = std::make_shared<Context>("");
};

/* ---------------------------------------------------------------------------- */

class Number : public Value
{
    public:
        Number(double value=0.0, std::shared_ptr<Context> context=nullptr, std::shared_ptr<Position> pos_start = nullptr, std::shared_ptr<Position> pos_end = nullptr);
        ~Number();

        /**
         * Returns value as string
         * @return std::string
        */
        std::string repr() override;

        std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> added_to(std::variant<std::shared_ptr<Value>, std::nullptr_t> other) override;
        std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> subbed_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other) override;
        std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> multed_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other) override;
        std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> dived_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other) override;
        std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> powed_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other) override;

        std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> get_comparison_eq(std::variant<std::shared_ptr<Value>, std::nullptr_t> other) override;
        std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> get_comparison_ne(std::variant<std::shared_ptr<Value>, std::nullptr_t> other) override;
        std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> get_comparison_lt(std::variant<std::shared_ptr<Value>, std::nullptr_t> other) override;
        std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> get_comparison_gt(std::variant<std::shared_ptr<Value>, std::nullptr_t> other) override;
        std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> get_comparison_lte(std::variant<std::shared_ptr<Value>, std::nullptr_t> other) override;
        std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> get_comparison_gte(std::variant<std::shared_ptr<Value>, std::nullptr_t> other) override;

        std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> anded_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other) override;
        std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> ored_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other) override;
        std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> notted_by() override;

        std::shared_ptr<Value> copy() override;
        bool is_true() override;
    public:
        std::shared_ptr<Error> error;
        double value = 0.0;
};

static const std::shared_ptr<Number> EMPTY_NUMBER;

/* ---------------------------------------------------------------------------- */

class String : public Value
{
    public:
        String(std::string value="", std::shared_ptr<Context> context=nullptr, std::shared_ptr<Position> pos_start = nullptr, std::shared_ptr<Position> pos_end = nullptr);
        ~String();

        /**
         * Returns value as string
         * @return std::string
        */
        std::string repr() override;

        std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> added_to(std::variant<std::shared_ptr<Value>, std::nullptr_t> other) override;
        std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> multed_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other) override;

        bool is_true() override;
        std::shared_ptr<Value> copy() override;

    public:
        std::string value = "";

    private:
        std::string multiply_string(std::string str, double times);
};

/* ---------------------------------------------------------------------------- */

class List : public Value
{
    public:
        List(std::vector<std::shared_ptr<Value>> elements, std::shared_ptr<Context> context=nullptr, std::shared_ptr<Position> pos_start = nullptr, std::shared_ptr<Position> pos_end = nullptr);
        ~List();

        /**
         * Returns value as string
         * @return std::string
        */
        std::string repr() override;

        std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> added_to(std::variant<std::shared_ptr<Value>, std::nullptr_t> other) override;
        std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> subbed_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other) override;
        std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> multed_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other) override;
        std::tuple<std::shared_ptr<Value>, std::shared_ptr<Error>> dived_by(std::variant<std::shared_ptr<Value>, std::nullptr_t> other) override;

        std::shared_ptr<Value> copy() override;

    public:
        std::vector<std::shared_ptr<Value>> elements;
};

/* ---------------------------------------------------------------------------- */

class BaseFunction : virtual public Value
{
    public:
        BaseFunction(std::string name, std::shared_ptr<Context> context=nullptr, std::shared_ptr<Position> pos_start = nullptr, std::shared_ptr<Position> pos_end = nullptr);
        ~BaseFunction();

        std::shared_ptr<Context> generate_new_context();
        std::shared_ptr<RTResult> check_args(std::vector<std::string> arg_names, std::vector<std::shared_ptr<Value>> args);
        void populate_args(std::vector<std::string> arg_names, std::vector<std::shared_ptr<Value>> args, std::shared_ptr<Context> exec_ctx);
        std::shared_ptr<RTResult> check_and_populate_args(std::vector<std::string> arg_names, std::vector<std::shared_ptr<Value>> args, std::shared_ptr<Context> exec_ctx);

    public:
        std::string name;
};

/* ---------------------------------------------------------------------------- */

class Function : public BaseFunction
{
    public:
        Function(std::string name, ALL_VARIANT body_node, std::vector<std::string> arg_names, bool should_auto_return, std::shared_ptr<Context> context=nullptr, std::shared_ptr<Position> pos_start = nullptr, std::shared_ptr<Position> pos_end = nullptr);
        ~Function();

    public:
        /**
         * Returns value as string
         * @return std::string
        */
        std::string repr() override;

        std::shared_ptr<RTResult> execute(std::vector<std::shared_ptr<Value>> args) override;
        std::shared_ptr<Value> copy() override;

    public:
        std::string name = "";
        ALL_VARIANT body_node;
        std::vector<std::string> arg_names;
        bool should_auto_return;
};

/* ---------------------------------------------------------------------------- */

class BuiltInFunction : public BaseFunction
{
    public:
        BuiltInFunction(std::string name, std::shared_ptr<Context> context=nullptr, std::shared_ptr<Position> pos_start = nullptr, std::shared_ptr<Position> pos_end = nullptr);
        ~BuiltInFunction();

        /**
         * Returns value as string
         * @return std::string
        */
        std::string repr() override;

        std::shared_ptr<RTResult> execute(std::vector<std::shared_ptr<Value>> args) override;
        std::shared_ptr<Value> copy() override;

        std::shared_ptr<RTResult> execute_clear(std::shared_ptr<Context> exec_ctx);
        std::shared_ptr<RTResult> execute_print(std::shared_ptr<Context> exec_ctx);
        std::shared_ptr<RTResult> execute_write(std::shared_ptr<Context> exec_ctx);

    private:
        std::vector<std::string> arg_names;
};

template <typename T>
void extend(std::vector<T>& vec, const std::vector<T>& elements);
