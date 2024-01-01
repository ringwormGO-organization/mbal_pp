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

        std::shared_ptr<Value> register_result(std::shared_ptr<RTResult> res);
        std::shared_ptr<RTResult> success(std::shared_ptr<Value> value);
        std::shared_ptr<RTResult> failure(std::shared_ptr<Error> error);

    public:
        std::shared_ptr<Value> value;
        std::shared_ptr<Error> error = std::make_shared<Error>(std::make_shared<Position>(0, 0, 0, "", ""), std::make_shared<Position>(0, 0, 0, "", ""), "", "");
};
