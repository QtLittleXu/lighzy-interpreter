#pragma once

#include "basic/Expression.hpp"
#include "BlockStatement.hpp"
#include "ArgumentsStatement.hpp"
#include <memory>
#include <sstream>

namespace li
{


class FunctionExpression : public Expression
{
public:
	FunctionExpression(const shared_ptr<Token>& token) :
		Expression(token, Node::Type::Function) {}

	void setBody(const shared_ptr<BlockStatement>& body)
	{
		_body = body;
	}

	const auto& body() const
	{
		return _body;
	}

	void setArgs(const shared_ptr<ArgumentsStatement>& args)
	{
		_args = args;
	}

	const auto& args() const
	{
		return _args;
	}

	string toString() const override
	{
		stringstream buffer;
		buffer << "fun" << _args->toString() << " " << _body->toString();
		return buffer.str();
	}

private:
	shared_ptr<BlockStatement> _body;
	shared_ptr<ArgumentsStatement> _args;
};


}