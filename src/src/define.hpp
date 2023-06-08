/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++, simple game inspired by Uno in terminal
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#include <memory>
#include <variant>

#include "Nodes.hpp"

#pragma once

#define ALL_VARIANT std::variant<std::shared_ptr<NumberNode>, std::shared_ptr<BinOpNode>, std::shared_ptr<UnaryOpNode>>