#pragma once

#include "basic/Expression.hpp"

namespace li
{


class InDecrementExpr : public Expr
{
public:
    InDecrementExpr(const shared_ptr<Token>& token) :
        Expr(token, Type::InDecrement) {}

	string toString() const override
	{
		stringstream buffer;
		buffer << operatorName << ' ' << id->toString();
		return buffer.str();
	}

public:
	shared_ptr<Expr> id;
	string operatorName;
};


}
