/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Error.hpp"

Error::Error(std::string error_name, std::string details)
{
    this->error_name = error_name;
    this->details = details;
}

Error::~Error()
{
    
}

std::string Error::as_string()
{
    return ((this->error_name += ": ") += this->details);
}
