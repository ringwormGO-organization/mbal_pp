/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "RTResult.hpp"

RTResult::RTResult()
{

}

RTResult::~RTResult()
{

}

void RTResult::reset()
{
    this->value = empty_value;
    this->error = std::make_shared<Error>(std::make_shared<Position>(0, 0, 0, "", ""), std::make_shared<Position>(0, 0, 0, "", ""), "", "");
    this->func_return_value = empty_value;
    this->loop_should_continue = false;
    this->loop_should_break = false;
}

std::shared_ptr<Value> RTResult::register_result(std::shared_ptr<RTResult> res)
{
    this->error = res->error;
    this->func_return_value = res->func_return_value;
    this->loop_should_continue = res->loop_should_continue;
    this->loop_should_break = res->loop_should_break;

    return res->value;
}

std::shared_ptr<RTResult> RTResult::success(std::shared_ptr<Value> value)
{
    this->reset();
    this->value = value;
    
    return std::shared_ptr<RTResult>(shared_from_this());
}

std::shared_ptr<RTResult> RTResult::success_return(std::shared_ptr<Value> value)
{
    this->reset();
    this->func_return_value = value;
}

std::shared_ptr<RTResult> RTResult::success_continue()
{
    this->reset();
    this->loop_should_continue = true;
}

std::shared_ptr<RTResult> RTResult::success_break()
{
    this->reset();
    this->loop_should_break = true;
}

std::shared_ptr<RTResult> RTResult::failure(std::shared_ptr<Error> error)
{
    this->reset();
    this->error = error;

    return std::shared_ptr<RTResult>(shared_from_this());
}

bool RTResult::should_return()
{
    if (this->error->error_name != "") { return true; }
    if (func_return_value != nullptr) { return true; }
    if (loop_should_continue) { return true; }
    if (loop_should_break) { return true; }

    return false;
}
