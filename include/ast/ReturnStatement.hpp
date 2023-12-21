#pragma once

#include "basic/Expression.hpp"
#include "basic/Statement.hpp"
#include <memory>
#include <sstream>

namespace li
{


class ReturnStatement : public Statement
{
public:
	ReturnStatement(const shared_ptr<Token>& token) :
		Statement(token, Node::Type::Return) {}

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
		stringstream stream;
		stream << _token->literal() << " " << _value->toString();
		return stream.str();
	}

private:
	shared_ptr<Expression> _value;
};


}
