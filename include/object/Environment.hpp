#pragma once

#include "basic/Object.h"

namespace li
{


class Environment
{
public:
	Environment(const shared_ptr<Environment>& outter = nullptr) : _outer(outter) {}

	void set(const string& name, const shared_ptr<Object>& value)
	{
		_store[name] = value;
	}

	shared_ptr<Object> get(const string& name)
	{
		auto it = _store.find(name);

		if (it == _store.end())
		{
			if (!_outer)
			{
				return nullptr;
			}
			return _outer->get(name);
		}
		return it->second;
	}

	void setOutter(const shared_ptr<Environment>& env)
	{
		_outer = env;
	}

	const auto& outer() const
	{
		return _outer;
	}

private:
	map<string, shared_ptr<Object>> _store;
	shared_ptr<Environment> _outer;
};


}