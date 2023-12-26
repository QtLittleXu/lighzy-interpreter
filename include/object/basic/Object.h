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
	Object(Type type) : type(type) {}

	static string typeName(Type type)
	{
		auto it = typeNames.find(type);
		if (it == typeNames.end())
		{
			return "";
		}
		return it->second;
	}

	string typeName() const
	{
		return typeName(type);
	}

public:
	Type type;
	static const map<Type, string> typeNames;
};


}