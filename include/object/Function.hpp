#pragma once

#include "basic/Object.h"
#include "Environment.hpp"
#include "ast/ArgumentsStat.hpp"
#include "ast/BlockStat.hpp"

namespace li
{

class Function : public Object
{
public:
	Function(shared_ptr<ArgumentsStat> args = nullptr,
			 shared_ptr<BlockStat> body = nullptr,
			 shared_ptr<Environment> env = nullptr) :
	Object(Type::Function), args(args), body(body), env(env) {}

	string inspect() const override
	{
		stringstream buffer;
		buffer << "fun" << args->toString() << body->toString();
		return buffer.str();
	}

public:
	shared_ptr<ArgumentsStat> args;
	shared_ptr<BlockStat> body;
	shared_ptr<Environment> env;
};

} // namespace li