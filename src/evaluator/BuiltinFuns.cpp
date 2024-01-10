#include "evaluator/BuiltinFuns.h"
#include "evaluator/Evaluator.h"
#include "object/String.hpp"
#include <iostream>

namespace li
{

const map<string, shared_ptr<BuiltinFun>> Evaluator::builtinFuns = 
{
	{ "_builtin_", make_shared<BuiltinFun>(BuiltinFuns::_builtin_, "_builtin_") }
};

shared_ptr<Object> BuiltinFuns::_builtin_(const vector<shared_ptr<Object>>& objs)
{
	if (objs.empty())
	{
		return Evaluator::invalid_arguments("expected the type of first argument to be integer, but got empty");
	}
	if (objs.at(0)->type != Object::Type::Integer)
	{
		return Evaluator::invalid_arguments("expected the type of first argument to be integer, but got " + objs.at(0)->typeName());
	}

	//	Extract the second to last arguments
	vector<shared_ptr<Object>> extraction(objs.begin() + 1, objs.end());
	int64_t opcode = dynamic_pointer_cast<Integer>(objs.at(0))->value;
	switch (opcode)
	{

	case Print:
		return print(extraction);

	case Len:
		return len(extraction);

	default:
		return Evaluator::invalid_arguments("unknown opcode: " + to_string(opcode));
		
	}
}

shared_ptr<Object> BuiltinFuns::print(const vector<shared_ptr<Object>>& objs)
{
	if (objs.empty())
	{
		return Evaluator::invalid_arguments("expected the type of first argument to be integer, but got empty");
	}
	if (objs.size() != 2)
	{
		return Evaluator::invalid_arguments("expected the number of arguments to be 2, but got " + to_string(objs.size()));
	}
	if (objs.at(1)->type != Object::Type::Bool)
	{
		return Evaluator::invalid_arguments("expected the type of second argument to be bool, but got " + objs.at(0)->typeName());
	}

	cout << objs.at(0)->inspect();
	if (objs.at(1) == Evaluator::bool_true)
	{
		cout << '\n';
	}
	return Evaluator::null;
}

shared_ptr<Object> BuiltinFuns::len(const vector<shared_ptr<Object>>& objs)
{
	if (objs.size() != 1)
	{
		return Evaluator::invalid_arguments("expected the number of them to be 1, but got " + to_string(objs.size()));
	}

	switch (objs.at(0)->type)
	{
	
	case Object::Type::String:
	{
		auto cast = dynamic_pointer_cast<String>(objs.at(0));
		return make_shared<Integer>(cast->value.size());
	}

	case Object::Type::Array:
	{
		auto cast = dynamic_pointer_cast<Array>(objs.at(0));
		return make_shared<Integer>(cast->elements.size());
	}

	default:
		return Evaluator::invalid_arguments("unknown function for argument type " + objs.at(0)->typeName());

	}
}


}