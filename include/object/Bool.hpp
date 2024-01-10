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

	shared_ptr<Object> copy() override
	{
		return make_shared<Bool>(*this);
	}

public:
	bool value{};
};


}