/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Error.hpp"

Error::Error(Position pos_start, Position pos_end, std::string error_name, std::string details) : pos_start(pos_start), pos_end(pos_end)
{
    this->pos_start = pos_start;
    this->pos_end = pos_end;

    this->error_name = error_name;
    this->details = details;
}

std::string Error::as_string()
{
    return ((this->error_name += ": ") += this->details) += ((std::string("\nFile") += this->pos_start.fn) += std::string(", line ") += std::to_string(this->pos_start.ln + 1));
}
