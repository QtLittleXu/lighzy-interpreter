#pragma once

#include "basic/Object.h"

namespace li
{


class Float : public Object
{
public:
	Float(double value = 0) : Object(Type::Float), value(value) {}

	string inspect() const override
	{
		stringstream buffer;
		buffer << value;
		return buffer.str();
	}

	shared_ptr<Object> copy() override
	{
		return make_shared<Float>(*this);
	}

public:
	double value;
};


}