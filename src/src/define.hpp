/**
 * @author Andrej123456789 (Andrej Bartulin)
 * @project: mbal++
 * @license: ringwormGO General License 1.0 | (RGL) 2022
*/

#pragma once

#define ALL_VARIANT std::variant<std::shared_ptr<NumberNode>, std::shared_ptr<VarAccessNode>, std::shared_ptr<VarAssignNode>, std::shared_ptr<BinOpNode>, std::shared_ptr<UnaryOpNode>, std::shared_ptr<IfNode>, std::shared_ptr<ForNode>, std::shared_ptr<WhileNode>>
