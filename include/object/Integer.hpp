#pragma once

#include "basic/Object.h"

namespace li
{


class Integer : public Object
{
public:
	Integer(int64_t value = 0) : Object(Object::Type::Integer), _value(value) {}

	string inspect() const override
	{
		return to_string(_value);
	}

	void setValue(int64_t value)
	{
		_value = value;
	}

	auto value() const
	{
		return _value;
	}

private:
	int64_t _value{};
};


}