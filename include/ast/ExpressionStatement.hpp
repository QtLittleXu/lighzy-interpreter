#pragma once

#include "basic/Statement.hpp"
#include "basic/Expression.hpp"

namespace li
{


class ExpressionStatement : public Statement
{
public:
    ExpressionStatement(const shared_ptr<Token>& token) :
        Statement(token, Node::Type::ExpressionStatement) {}

    const auto& expression() const
    {
        return _expr;
    }

    void setExpression(const shared_ptr<Expression>& expr)
    {
        _expr = expr;
    }

    string toString() const override
    {
        return _expr->toString();
    }

private:
    shared_ptr<Expression> _expr;
};


}
