#pragma once

#include "basic/Statement.hpp"
#include "basic/Expression.hpp"

namespace li
{


class ExpressionsStat : public Stat
{
public:
    ExpressionsStat(const shared_ptr<Token>& token) :
        Stat(token, Type::Exprs) {}

    string toString() const override
    {
		stringstream buffer;
		buffer << "(";
		auto it = expressions.begin();
		for (; it != expressions.end() - 1; it++)
		{
			buffer << (*it)->toString() << ", ";
		}
		buffer << (*it)->toString() << ")";
		return buffer.str();
    }

public:
    vector<shared_ptr<Expr>> expressions;
};


}