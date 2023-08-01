/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "Error.hpp"
#include "Values.hpp"

class RTResult : public std::enable_shared_from_this<RTResult>
{
    public:
        RTResult();
        ~RTResult();

        void reset();
        std::shared_ptr<Value> register_result(std::shared_ptr<RTResult> res);

        std::shared_ptr<RTResult> success(std::shared_ptr<Value> value);
        std::shared_ptr<RTResult> success_return(std::shared_ptr<Value> value);
        std::shared_ptr<RTResult> success_continue();
        std::shared_ptr<RTResult> success_break();

        std::shared_ptr<RTResult> failure(std::shared_ptr<Error> error);
        bool should_return();

    public:
        std::shared_ptr<Value> value;
        std::shared_ptr<Error> error = std::make_shared<Error>(std::make_shared<Position>(0, 0, 0, "", ""), std::make_shared<Position>(0, 0, 0, "", ""), "", "");
        std::shared_ptr<Value> func_return_value;

        bool loop_should_continue = false;
        bool loop_should_break = false;

    private:
        std::shared_ptr<Value> empty_value;
};
