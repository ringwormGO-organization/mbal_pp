/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include "Error.hpp"

std::string string_with_arrows(std::string text, std::shared_ptr<Position> pos_start, std::shared_ptr<Position> pos_end)
{
    /* Declare variables */
    std::string result = "";

    std::size_t idx_start = 0;
    std::size_t idx_end = 0;

    /* Calculate indices */
    std::string::iterator begin = std::begin(text);
    std::string::iterator end = std::end(text) - pos_start->idx;

    std::string::iterator found = std::find(begin, end, '\n');
    if (found != end) 
    {
        idx_start = std::max(static_cast<std::size_t>(std::distance(std::begin(text), found)), static_cast<std::size_t>(0));
    }

    else
    {
        begin = std::begin(text);
        end = std::end(text) - (idx_start + 1);

        found = std::find(begin, end, '\n');

        if (found == end)
        {
            idx_end = text.length();
        }
    }

    /* Generate each line */
    std::size_t line_count = pos_end->ln - pos_start->ln + 1;
    for (std::size_t i = 0; i < line_count; i++)
    {
        /* Calculate line columns */
        std::string line = text.substr(idx_start, idx_end);

        signed long col_start = 0;
        (i == 0) ? col_start = pos_start->col : col_start = 0;

        signed long col_end = 0;
        (i == line_count - 1) ? col_end = pos_end->col : col_start = line.length() - 1;

        /* Append to result */
        result += line += "\n";
        
        for (signed long j = 0; j < col_start; j++)
        {
            result += " ";
        }
        result += " ";

        for (signed long k = 0; k < (col_end - col_start); k++)
        {
            result += "^";
        }
        
        /* Re-calculate indices */
        idx_start = idx_end;
        
        begin = std::begin(text);
        end = std::end(text) - (idx_start + 1);

        found = std::find(begin, end, '\n');

        if (found == end)
        {
            idx_end = text.length();
        }
    }

    std::replace(result.begin(), result.end(), '\t', ' ');
    return result;
}

Error::Error(std::shared_ptr<Position> pos_start, std::shared_ptr<Position> pos_end, std::string error_name, std::string details)
{
    this->pos_start = pos_start;
    this->pos_end = pos_end;

    this->error_name = error_name;
    this->details = details;
}

std::string Error::as_string()
{
    std::string result = (this->error_name += ": ") += this->details += "\n";

    result += (std::string("File ") += this->pos_start->fn) += (std::string(", line ") += std::to_string(this->pos_start->ln + 1));
    result += std::string("\n\n ") += string_with_arrows(this->pos_start->ftxt, this->pos_start, this->pos_end);

    return result;
}
