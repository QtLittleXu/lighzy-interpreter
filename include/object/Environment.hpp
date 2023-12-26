#pragma once

#include "basic/Object.h"

namespace li
{


class Environment
{
public:
	Environment(const shared_ptr<Environment>& outter = nullptr) : outer(outter) {}

	void set(const string& name, const shared_ptr<Object>& value)
	{
		store[name] = value;
	}

	shared_ptr<Object> get(const string& name)
	{
		auto it = store.find(name);

		if (it == store.end())
		{
			if (!outer)
			{
				return nullptr;
			}
			return outer->get(name);
		}
		return it->second;
	}

public:
	map<string, shared_ptr<Object>> store;
	shared_ptr<Environment> outer;
};


}