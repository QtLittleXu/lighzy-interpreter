#pragma once

#include "basic/Expression.hpp"
#include "BlockStat.hpp"
#include "ArgumentsStat.hpp"

namespace li
{


class FunctionExpr : public Expr
{
public:
	FunctionExpr(const shared_ptr<Token>& token) :
		Expr(token, Type::Function) {}

	string toString() const override
	{
		stringstream buffer;
		buffer << "fun" << args->toString() << " " << body->toString();
		return buffer.str();
	}

public:
	shared_ptr<BlockStat> body;
	shared_ptr<ArgumentsStat> args;
};


}