#pragma once

#include "basic/Statement.hpp"
#include "IdentifierExpr.hpp"
#include <memory>
#include <sstream>
#include <vector>

namespace li
{


class ArgumentsStat : public Stat
{
public:
	ArgumentsStat(shared_ptr<Token> token) : 
		Stat(token, Type::Arguments) {}

	string toString() const override
	{
		stringstream buffer;
		buffer << "(";
		auto it = args.begin();
		for (; it != args.end() - 1; it++)
		{
			buffer << (*it)->toString() << ", ";
		}
		buffer << (*it)->toString() << ")";
		return buffer.str();
	}

public:
	vector<shared_ptr<IdentifierExpr>> args;
};


}