#pragma once

#include "basic/Object.h"
#include <functional>

namespace li
{


class BuiltinFun : public Object
{
public:
	using built_in_fun = function<shared_ptr<Object>( const vector<shared_ptr<Object>>& )>;

public:
	BuiltinFun(const built_in_fun& fun = {}, const string& inspectText = "") : Object(Type::BuiltinFun), fun(fun), inspectText(inspectText) {}

	string inspect() const override
	{
		return inspectText;
	}

	shared_ptr<Object> copy() override
	{
		return make_shared<BuiltinFun>(*this);
	}

	void assign(shared_ptr<Object> value) override
	{
		*this = *dynamic_pointer_cast<BuiltinFun>(value);
	}

public:
	built_in_fun fun;
	string inspectText;
};


}