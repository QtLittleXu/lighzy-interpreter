#pragma once

#include "basic/Expression.hpp"

namespace li
{


class IndexExpr : public Expr
{
public:
    IndexExpr(shared_ptr<Token> token) :
        Expr(token, Type::Index) {}

    string toString() const override
    {
		stringstream buffer;
		buffer << left->toString() << "[" << index->toString() << "]";
		return buffer.str();
    }

public:
	shared_ptr<Expr> index;
	shared_ptr<Expr> left;
};


}
