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

	shared_ptr<Object> copy() override
	{
		return make_shared<String>(*this);
	}

	void assign(shared_ptr<Object> value) override
	{
		*this = *dynamic_pointer_cast<String>(value);
	}

public:
	string value;
};


}