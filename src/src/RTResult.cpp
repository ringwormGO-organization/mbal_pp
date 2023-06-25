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

std::shared_ptr<Value> RTResult::register_result(std::shared_ptr<RTResult> res)
{
    if (res->error->error_name != "")
    {
        this->error = res->error;
    }

    return res->value;
}

std::shared_ptr<RTResult> RTResult::success(std::shared_ptr<Value> value)
{
    this->value = value;
    return std::shared_ptr<RTResult>(shared_from_this());
}

std::shared_ptr<RTResult> RTResult::failure(std::shared_ptr<Error> error)
{
    this->error = error;
    return std::shared_ptr<RTResult>(shared_from_this());
}
