#pragma once

#include "basic/Statement.hpp"
#include "IdentifierExpression.hpp"
#include <memory>
#include <sstream>
#include <vector>

namespace li
{


class ArgumentsStatement : public Statement
{
public:
	ArgumentsStatement(const shared_ptr<Token>& token) : 
		Statement(token, Node::Type::Arguments) {}

	void addArg(const shared_ptr<IdentifierExpression>& arg)
	{
		_args.push_back(arg);
	}

	const auto& args() const
	{
		return _args;
	}

	string toString() const override
	{
		stringstream buffer;
		buffer << "(";
		auto it = _args.begin();
		for (; it != _args.end() - 1; it++)
		{
			buffer << (*it)->toString() << ", ";
		}
		buffer << (*it)->toString() << ")";
		return buffer.str();
	}

private:
	vector<shared_ptr<IdentifierExpression>> _args;
};


}