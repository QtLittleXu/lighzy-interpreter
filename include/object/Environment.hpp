#pragma once

#include "basic/Object.h"

namespace li
{


class Environment
{
public:
	Environment(const shared_ptr<Environment>& outer = nullptr) : outer(outer) {}

	// This function will set the value of name if name can be found, otherwise do nothing
	void set(const string& name, const shared_ptr<Object>& value)
	{
		auto* found = get(name);
		if (found != nullptr)
		{
			*found = value;
		}
	}

	void add(const string& name, const shared_ptr<Object>& value)
	{
		store.emplace(name, value);
	}

	shared_ptr<Object>* get(const string& name)
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
		return &it->second;
	}

public:
	map<string, shared_ptr<Object>> store;
	shared_ptr<Environment> outer;
};


}