#pragma once

#include "basic/Expression.hpp"

namespace li
{


class AssignExpr : public Expr
{
public:
    AssignExpr(shared_ptr<Token> token) :
        Expr(token, Type::Assign) {}

	string toString() const override
	{
		stringstream buffer;
		buffer << id->toString() << " = " << value->toString();
		return buffer.str();
	}

public:
	shared_ptr<Expr> id;
	string operatorName;
	shared_ptr<Expr> value;
};


}
