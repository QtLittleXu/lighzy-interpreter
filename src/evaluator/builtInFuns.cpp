#include "evaluator/Evaluator.h"
#include "object/Integer.hpp"
#include "object/String.hpp"
#include <iostream>

namespace li
{


const map<string, shared_ptr<BuiltInFun>> Evaluator::builtInFuns = {
	{ "len", make_shared<BuiltInFun>(Evaluator::len, "len") },
	{ "puts", make_shared<BuiltInFun>(Evaluator::puts, "puts") }
};

shared_ptr<Object> Evaluator::len(const vector<shared_ptr<Object>>& objs)
{
	if (objs.size() != 1)
	{
		return invalid_arguments(string("expected the number of them to be 1, but got ") + to_string(objs.size()));
	}
	if (objs.at(0)->type != Object::Type::String)
	{
		return invalid_arguments(string("expected the type of them to be string, but got ") + objs.at(0)->typeName());
	}

	auto value = dynamic_pointer_cast<String>(objs.at(0))->value;
	return make_shared<Integer>(value.size());
}

shared_ptr<Object> Evaluator::puts(const vector<shared_ptr<Object>>& objs)
{
	for (const auto& obj : objs)
	{
		cout << obj->inspect() << '\n';
	}
	return null;
}


}