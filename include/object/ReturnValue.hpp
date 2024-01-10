#pragma once

#include "basic/Object.h"

namespace li
{


class ReturnValue : public Object
{
public:
	ReturnValue(shared_ptr<Object> value = nullptr) : Object(Type::ReturnValue), value(value) {}

	string inspect() const override
	{
		return value->inspect();
	}

	shared_ptr<Object> copy() override
	{
		return make_shared<ReturnValue>(*this);
	}

public:
	shared_ptr<Object> value;
};


}