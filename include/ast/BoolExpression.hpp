#pragma once

#include "basic/Expression.hpp"

namespace li
{


class BoolExpression : public Expression
{
public:
    BoolExpression(const shared_ptr<Token>& token) :
        Expression(token, Node::Type::Bool) {}

    void setValue(bool value)
    {
        _value = value;
    }

    const auto& value() const
    {
        return _value;
    }

    string toString() const override
    {
        return _value ? "true" : "false";
    }

private:
    bool _value;
};


}
