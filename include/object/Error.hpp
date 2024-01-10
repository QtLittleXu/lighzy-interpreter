#pragma once

#include "basic/Object.h"

namespace li
{

class Error : public Object
{
public:
	Error(const string& msg = "") : Object(Type::Error), message(msg) {}

	string inspect() const override
	{
		return message;
	}

	shared_ptr<Object> copy() override
	{
		return make_shared<Error>(*this);
	}

public:
	string message;
};

} // namespace li