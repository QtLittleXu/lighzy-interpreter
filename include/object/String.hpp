#pragma once

#include "basic/Object.h"

namespace li
{


class String : public Object
{
public:
	String(const string& value = "") : Object(Object::Type::String), _value(value) {}

	string inspect() const override
	{
		return _value;
	}

	void setValue(const string& value)
	{
		_value = value;
	}

	const auto& value() const
	{
		return _value;
	}

private:
	string _value;
};


}