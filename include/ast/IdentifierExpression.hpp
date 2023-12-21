#pragma once

#include "basic/Expression.hpp"

namespace li
{


class IdentifierExpression : public Expression
{
public:
    IdentifierExpression(const shared_ptr<Token>& token) :
        Expression(token, Node::Type::Identifier) {}

    const string& value() const
    {
        return _value;
    }

    void setValue(const string& value)
    {
        _value = value;
    }

    string toString() const override
    {
        return _value;
    }

private:
    string _value;
};


}
