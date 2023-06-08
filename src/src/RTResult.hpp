/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "Values.hpp"

class RTResult : public std::enable_shared_from_this<RTResult>
{
    public:
        RTResult();
        ~RTResult();

        std::shared_ptr<Number> register_result(std::shared_ptr<RTResult> res);
        std::shared_ptr<RTResult> success(std::shared_ptr<Number> value);
        std::shared_ptr<RTResult> failure(Error error);

    public:
        std::shared_ptr<Number> value;
        Error error;
};
