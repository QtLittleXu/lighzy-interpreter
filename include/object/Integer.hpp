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

	shared_ptr<Object> copy() override
	{
		return make_shared<Integer>(*this);
	}
	
public:
	int64_t value;
};


}