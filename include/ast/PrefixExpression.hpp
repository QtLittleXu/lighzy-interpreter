#pragma once

#include "basic/Expression.hpp"
#include <sstream>

namespace li
{


class PrefixExpression : public Expression
{
public:
    PrefixExpression(const shared_ptr<Token>& token) :
        Expression(token, Node::Type::Prefix) {}

    void setRight(const shared_ptr<Expression>& expr)
    {
        _right = expr;
    }

    const auto& right() const
    {
        return _right;
    }

    const string& operatorName()
    {
        return _operator;
    }

    void setOperator(const string& operatorName)
    {
        _operator = operatorName;
    }

    string toString() const override
    {
        stringstream stream;
        stream << _operator << _right->toString();
        return stream.str();
    }

private:
    shared_ptr<Expression> _right;
    string _operator;
};


}
