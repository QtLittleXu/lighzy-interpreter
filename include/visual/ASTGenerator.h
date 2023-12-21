#pragma once

#include <nlohmann/json.hpp>
#include "ast/basic/Node.hpp"

namespace li::visual
{

using namespace nlohmann;


class ASTGenerator
{
public:
	static json generateJson(const shared_ptr<Node>& node);
};


}