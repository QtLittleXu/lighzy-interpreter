#pragma once

#include "basic/Object.h"

namespace li
{

class Null : public Object
{
public:
	Null() : Object(Type::Null) {}

	string inspect() const override
	{
		return "null";
	}
};

} // namespace li