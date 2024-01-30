#pragma once

#include "basic/Expression.hpp"
#include "BlockStat.hpp"
#include <string>

namespace li
{


class IfExpr : public Expr
{
public:	
	IfExpr(shared_ptr<Token> token) :
		Expr(token, Type::If) {}

	string toString() const override
	{
		stringstream buffer;
		buffer << "if (" << condition->toString() << ") " << consequence->toString();
		if (alternative)
		{
			buffer << " else " << alternative->toString();
		}
		return buffer.str();
	}

public:
	shared_ptr<Expr> condition;
	shared_ptr<BlockStat> consequence;
	shared_ptr<BlockStat> alternative;
};


}