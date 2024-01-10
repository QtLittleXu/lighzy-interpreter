#pragma once

#include "basic/Object.h"

namespace li
{


class Array : public Object
{
public:
	Array(const vector<shared_ptr<Object>>& elements = {}) : Object(Type::Array), elements(elements) {}

	string inspect() const override
	{
		stringstream buffer;
		buffer << "[";
		int i = 0;
		while (i < elements.size() - 1)
		{
			buffer << elements.at(i)->inspect() << ", ";
			i++;
		}
		buffer << elements.at(i)->inspect() << "]";
		return buffer.str();
	}

	shared_ptr<Object> copy() override
	{
		auto copied = make_shared<Array>();
		for (const auto& element : elements)
		{
			copied->elements.push_back(element->copy());
		}
		return copied;
	}

	void assign(shared_ptr<Object> value) override
	{
		for (const auto& element : dynamic_pointer_cast<Array>(value)->elements)
		{
			elements.push_back(element);
		}
	}

public:
	vector<shared_ptr<Object>> elements;
};


}