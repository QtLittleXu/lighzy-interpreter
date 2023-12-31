#pragma once

#include "basic/Object.h"
#include <optional>

namespace li
{


class Environment
{
public:
	Environment(const shared_ptr<Environment>& outter = nullptr) : outer(outter) {}

	// This function will set the value of name if name can be found, otherwise add the name in current env
	void set(const string& name, const shared_ptr<Object>& value)
	{
		auto it = store.find(name);
		bool isNotFoundInCurrentEnv = it == store.end();
		bool isOuterAvailable = outer != nullptr;

		if (isNotFoundInCurrentEnv && isOuterAvailable)
		{
			outer->set(name, value);
			return;
		}
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