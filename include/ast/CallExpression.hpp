#pragma once

#include "basic/Expression.hpp"
#include "ExpressionsStatement.hpp"
#include <memory>

namespace li
{


class CallExpression : public Expression
{
public:
	CallExpression(const shared_ptr<Token>& token) :
		Expression(token, Node::Type::Call) {}

	void setExprs(const shared_ptr<ExpressionsStatement>& exprs)
	{
		_exprs = exprs;
	}

	const auto& exprs() const
	{
		return _exprs;
	}

	void setFun(const shared_ptr<Expression>& fun)
	{
		_fun = fun;
	}

	const auto& fun() const
	{
		return _fun;
	}

	string toString() const override
	{
		stringstream buffer;
		buffer << _fun->toString() << _exprs->toString();
		return buffer.str();
	}

private:
	shared_ptr<ExpressionsStatement> _exprs;
	shared_ptr<Expression> _fun;
};


}