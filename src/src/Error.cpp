/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Error.hpp"

std::string string_with_arrows(std::string text, Position pos_start, Position pos_end)
{
    // TODO
    return text;
}

Error::Error(Position pos_start, Position pos_end, std::string error_name, std::string details) : pos_start(pos_start), pos_end(pos_end)
{
    this->pos_start = pos_start;
    this->pos_end = pos_end;

    this->error_name = error_name;
    this->details = details;
}

std::string Error::as_string()
{
    std::string result = (this->error_name += ": ") += this->details += "\n";

    result += (std::string("File ") += this->pos_start.fn) += (std::string(", line ") += std::to_string(this->pos_start.ln + 1));
    result += std::string("\n\n ") += string_with_arrows(this->pos_start.ftxt, this->pos_start, this->pos_end);

    return result;
}
