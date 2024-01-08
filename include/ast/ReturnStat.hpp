#pragma once

#include "basic/Expression.hpp"
#include "basic/Statement.hpp"
#include <memory>
#include <sstream>

namespace li
{


class ReturnStat : public Stat
{
public:
	ReturnStat(shared_ptr<Token> token) :
		Stat(token, Node::Type::Return) {}

	string toString() const override
	{
		stringstream stream;
		stream << token->literal << " " << value->toString();
		return stream.str();
	}

public:
	shared_ptr<Expr> value;
};


}
