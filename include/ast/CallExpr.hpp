#pragma once

#include "basic/Expression.hpp"
#include "ExpressionsStat.hpp"
#include <memory>

namespace li
{


class CallExpr : public Expr
{
public:
	CallExpr(shared_ptr<Token> token) :
		Expr(token, Type::Call) {}

	string toString() const override
	{
		stringstream buffer;
		buffer << fun->toString() << exprs->toString();
		return buffer.str();
	}

public:
	shared_ptr<ExpressionsStat> exprs;
	shared_ptr<Expr> fun;
};


}