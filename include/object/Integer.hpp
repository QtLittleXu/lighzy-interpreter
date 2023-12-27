#pragma once

#include "basic/Object.h"

namespace li
{


class Integer : public Object
{
public:
	Integer(int64_t value = 0) : Object(Type::Integer), value(value) {}

	string inspect() const override
	{
		return to_string(value);
	}

public:
	int64_t value;
};


}