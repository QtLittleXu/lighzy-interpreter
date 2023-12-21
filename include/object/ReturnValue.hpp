#pragma once

#include "basic/Object.h"

namespace li
{


class ReturnValue : public Object
{
public:
	ReturnValue(const shared_ptr<Object> value = nullptr) : Object(Object::Type::ReturnValue), _value(value) {}

	string inspect() const override
	{
		return _value->inspect();
	}

	void setValue(const shared_ptr<Object> value)
	{
		_value = value;
	}

	const auto& value() const
	{
		return _value;
	}

private:
	shared_ptr<Object> _value;
};


}