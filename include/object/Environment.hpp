#pragma once

#include "basic/Object.h"

namespace li
{


class Environment
{
public:
	void set(const string& name, const shared_ptr<Object>& value)
	{
		_store[name] = value;
	}

	shared_ptr<Object> get(const string& name)
	{
		auto it = _store.find(name);
		if (it == _store.end())
		{
			return nullptr;
		}
		return it->second;
	}

private:
	map<string, shared_ptr<Object>> _store;
};


}