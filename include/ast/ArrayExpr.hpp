#pragma once

#include "basic/Expression.hpp"
#include "ExpressionsStat.hpp"

namespace li
{


class ArrayExpr : public Expr
{
public:
    ArrayExpr(shared_ptr<Token> token) :
        Expr(token, Type::Array) {}

    string toString() const override
    {
		stringstream buffer;
		buffer << "[";
		int i = 0;
		while (i < elements->expressions.size() - 1)
		{
			auto element = elements->expressions.at(i);
			buffer << element->toString() << ", ";
			i++;
		}
		buffer << elements->expressions.at(i)->toString() << "]";
		return buffer.str();
    }

public:
	shared_ptr<ExpressionsStat> elements;
};


}
