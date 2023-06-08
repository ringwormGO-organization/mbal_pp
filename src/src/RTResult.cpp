/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "RTResult.hpp"

RTResult::RTResult() : error(EMPTY_POSITION, EMPTY_POSITION, "", "")
{

}

RTResult::~RTResult()
{

}

std::shared_ptr<Number> RTResult::register_result(std::shared_ptr<RTResult> res)
{
    if (res->error.error_name != "")
    {
        this->error = res->error;
    }

    return res->value;
}

std::shared_ptr<RTResult> RTResult::success(std::shared_ptr<Number> value)
{
    this->value = value;
    return std::shared_ptr<RTResult>(shared_from_this());
}

std::shared_ptr<RTResult> RTResult::failure(Error error)
{
    this->error = error;
    return std::shared_ptr<RTResult>(shared_from_this());
}
