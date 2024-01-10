#pragma once

#include "basic/Object.h"

namespace li
{

class Null : public Object
{
public:
	Null() : Object(Type::Null) {}

	string inspect() const override
	{
		return "null";
	}

	shared_ptr<Object> copy() override
	{
		return make_shared<Null>(*this);
	}

	void assign(shared_ptr<Object> value) override
	{
		*this = *dynamic_pointer_cast<Null>(value);
	}
};

} // namespace li