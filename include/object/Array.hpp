#pragma once

#include "basic/Object.h"

namespace li
{


class Array : public Object
{
public:
	Array(const vector<shared_ptr<Object>>& elements) : Object(Type::Array), elements(elements) {}

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
		return make_shared<Array>(*this);
	}

public:
	vector<shared_ptr<Object>> elements;
};


}