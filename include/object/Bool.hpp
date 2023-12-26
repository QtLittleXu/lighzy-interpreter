#pragma once

#include "basic/Object.h"

namespace li
{


class Bool : public Object
{
public:
	Bool(bool value = false) : Object(Type::Bool), value(value) {}

	string inspect() const override
	{
		return value ? "true" : "false";
	}

public:
	bool value{};
};


}