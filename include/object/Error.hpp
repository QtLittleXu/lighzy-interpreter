#pragma once

#include "basic/Object.h"

namespace li
{

class Error : public Object
{
public:
	Error(const string& message = "") : Object(Object::Type::Error), _message(message) {}

	string inspect() const override
	{
		return _message;
	}

	void setMessage(const string& message)
	{
		_message = message;
	}

	const auto& message() const
	{
		return _message;
	}

private:
	string _message;
};

} // namespace li