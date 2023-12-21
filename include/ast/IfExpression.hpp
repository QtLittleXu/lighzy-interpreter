#pragma once

#include "basic/Expression.hpp"
#include "BlockStatement.hpp"
#include <string>

namespace li
{


class IfExpression : public Expression
{
public:	
	IfExpression(const shared_ptr<Token>& token) :
		Expression(token, Node::Type::If) {}

	void setCondition(const shared_ptr<Expression>& condition)
	{
		_condition = condition;
	}

	const auto& condition() const
	{
		return _condition;
	}

	void setConsequence(const shared_ptr<BlockStatement>& consequence)
	{
		_consequence = consequence;
	}

	const auto& consequence() const
	{
		return _consequence;
	}
	
	void setAlternative(const shared_ptr<BlockStatement>& alternative)
	{
		_alternative = alternative;
	}

	const auto& alternative() const
	{
		return _alternative;
	}

	string toString() const override
	{
		stringstream buffer;
		buffer << "if (" << _condition->toString() << ") " << _consequence->toString();
		if (_alternative)
		{
			buffer << " else " << _alternative->toString();
		}
		return buffer.str();
	}

private:
	shared_ptr<Expression> _condition;
	shared_ptr<BlockStatement> _consequence;
	shared_ptr<BlockStatement> _alternative;
};


}