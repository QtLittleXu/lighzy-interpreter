#pragma once

#include "basic/Object.h"

namespace li
{


class Bool : public Object
{
public:
	Bool(bool value = false) : Object(Object::Type::Bool), _value(value) {}

	string inspect() const override
	{
		return _value ? "true" : "false";
	}

	void setValue(bool value)
	{
		_value = value;
	}

	auto value() const
	{
		return _value;
	}

private:
	bool _value{};
};


}