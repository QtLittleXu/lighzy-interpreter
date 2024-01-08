#pragma once

#include "object/basic/Object.h"

namespace li
{


class BuiltinFuns
{
public:
	enum BuiltInType
	{
		Print, Len
	};

public:
	static shared_ptr<Object> _builtin_(const vector<shared_ptr<Object>> objs);

private:
	static shared_ptr<Object> print(const vector<shared_ptr<Object>> objs);
	static shared_ptr<Object> len(const vector<shared_ptr<Object>> objs);
};


}