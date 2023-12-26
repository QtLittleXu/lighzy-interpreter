#pragma once

#include "basic/Object.h"

namespace li
{


class String : public Object
{
public:
	String(const string& value = "") : Object(Type::String), value(value) {}

	string inspect() const override
	{
		return value;
	}

public:
	string value;
};


}