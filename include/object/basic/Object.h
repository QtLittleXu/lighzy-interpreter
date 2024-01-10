#pragma once

#include "ast/basic/Node.hpp"

namespace li
{


class Object
{
public:
	enum class Type
	{
		Null, Integer, Float, Bool, ReturnValue, Error, Function, String, BuiltinFun, Array
	};

public:
	virtual string inspect() const = 0;
	virtual shared_ptr<Object> copy() = 0;
	virtual void assign(shared_ptr<Object> value) = 0;

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