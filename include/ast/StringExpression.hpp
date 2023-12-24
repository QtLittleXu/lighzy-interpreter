#pragma once

#include "basic/Expression.hpp"

namespace li
{


class StringExpression : public Expression
{
public:
    StringExpression(const shared_ptr<Token>& token) :
        Expression(token, Node::Type::String) {}

    void setValue(string value)
    {
        _value = value;
    }

    const auto& value() const
    {
        return _value;
    }

    string toString() const override
    {
        return _value;
    }

private:
    string _value;
};


}
