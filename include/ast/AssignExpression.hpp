#pragma once

#include "basic/Expression.hpp"
#include "IdentifierExpression.hpp"

namespace li
{


class AssignExpression : public Expression
{
public:
    AssignExpression(const shared_ptr<Token>& token) :
        Expression(token, Node::Type::Assign) {}

	void setId(const shared_ptr<Expression>& id)
	{
		_id = id;
	}

	const auto& id() const
	{
		return _id;
	}

	void setValue(const shared_ptr<Expression>& value)
	{
		_value = value;
	}

	const auto& value() const
	{
		return _value;
	}

	string toString() const override
	{
		stringstream buffer;
		buffer << _id->toString() << " = " << _value->toString();
		return buffer.str();
	}

private:
	shared_ptr<Expression> _id;
	shared_ptr<Expression> _value;
};


}
