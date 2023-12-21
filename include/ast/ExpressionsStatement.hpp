#pragma once

#include "basic/Statement.hpp"
#include "basic/Expression.hpp"

namespace li
{


class ExpressionsStatement : public Statement
{
public:
    ExpressionsStatement(const shared_ptr<Token>& token) :
        Statement(token, Node::Type::Expressions) {}

    const auto& expressions() const
    {
        return _exprs;
    }

    void addExpression(const shared_ptr<Expression>& expr)
    {
        _exprs.push_back(expr);
    }

    string toString() const override
    {
		stringstream buffer;
		buffer << "(";
		auto it = _exprs.begin();
		for (; it != _exprs.end() - 1; it++)
		{
			buffer << (*it)->toString() << ", ";
		}
		buffer << (*it)->toString() << ")";
		return buffer.str();
    }

private:
    vector<shared_ptr<Expression>> _exprs;
};


}
