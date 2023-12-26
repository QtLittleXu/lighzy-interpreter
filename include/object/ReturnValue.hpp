#pragma once

#include "basic/Object.h"

namespace li
{


class ReturnValue : public Object
{
public:
	ReturnValue(const shared_ptr<Object>& value = nullptr) : Object(Type::ReturnValue), value(value) {}

	string inspect() const override
	{
		return value->inspect();
	}

public:
	shared_ptr<Object> value;
};


}