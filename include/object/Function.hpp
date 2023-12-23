#pragma once

#include "basic/Object.h"
#include "Environment.hpp"
#include "ast/ArgumentsStatement.hpp"
#include "ast/BlockStatement.hpp"

namespace li
{

class Function : public Object
{
public:
	Function(const shared_ptr<ArgumentsStatement>& args = nullptr, 
			 const shared_ptr<BlockStatement>& body = nullptr,
			 const shared_ptr<Environment>& env = nullptr) :
			Object(Object::Type::Function), _args(args), _body(body), _env(env) {}

	void setArgs(const shared_ptr<ArgumentsStatement>& args)
	{
		_args = args;
	}

	const auto& args() const
	{
		return _args;
	}

	void setBody(const shared_ptr<BlockStatement>& body)
	{
		_body = body;
	}

	const auto& body() const
	{
		return _body;
	}

	void setEnv(const shared_ptr<Environment>& env)
	{
		_env = env;
	}

	const auto& env() const
	{
		return _env;
	}

	string inspect() const override
	{
		stringstream buffer;
		buffer << "fun" << _args->toString() << _body->toString();
		return buffer.str();
	}

private:
	shared_ptr<ArgumentsStatement> _args;
	shared_ptr<BlockStatement> _body;
	shared_ptr<Environment> _env;
};

} // namespace li