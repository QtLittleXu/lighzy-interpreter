#pragma once

#include "basic/Expression.hpp"
#include <string>

namespace li
{


class InfixExpression : public Expression
{
public:
    InfixExpression(const shared_ptr<Token>& token) :
        Expression(token, Node::Type::Infix) {}

    void setLeft(const shared_ptr<Expression>& expr)
    {
        _left = expr;
    }

    const auto& left() const
    {
        return _left;
    }

    void setRight(const shared_ptr<Expression>& expr)
    {
        _right = expr;
    }

    const auto& right() const
    {
        return _right;
    }

    const string& operatorName() const
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
        stream << "("  << _left->toString() << " " << _operator << " " << _right->toString() << ")";
        return stream.str();
    }

private:
    shared_ptr<Expression> _left;
    shared_ptr<Expression> _right;
    string _operator;
};


}
