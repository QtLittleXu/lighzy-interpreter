#pragma once

#include "ast/basic/Node.hpp"

namespace li
{


class Object
{
public:
	enum class Type
	{
		Null, Integer, Bool, ReturnValue, Error, Function, String
	};

public:
	virtual string inspect() const = 0;

public:
	Object(Type type) : _type(type) {}

	auto type() const
	{
		return _type;
	}

	static string typeName(Type type)
	{
		auto it = _typeNames.find(type);
		if (it == _typeNames.end())
		{
			return "";
		}
		return it->second;
	}

	string typeName() const
	{
		return typeName(_type);
	}

protected:
	Type _type;
	static const map<Type, string> _typeNames;
};


}