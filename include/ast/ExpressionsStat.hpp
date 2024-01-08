#pragma once

#include "basic/Statement.hpp"
#include "basic/Expression.hpp"

namespace li
{


class ExpressionsStat : public Stat
{
public:
    ExpressionsStat(shared_ptr<Token> token) :
        Stat(token, Type::Exprs) {}

    string toString() const override
    {
		stringstream buffer;
		buffer << "(";
		for (const auto& expr : expressions)
		{
			buffer << expr->toString();
			if (expr != expressions.back())
			{
				buffer << ", ";
			}
		}
		buffer << ")";
		return buffer.str();
    }

public:
    vector<shared_ptr<Expr>> expressions;
};


}
