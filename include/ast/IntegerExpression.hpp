#pragma once

#include "basic/Expression.hpp"
#include <string>

namespace li
{


class IntegerExpression : public Expression
{
public:
    IntegerExpression(const shared_ptr<Token>& token) :
        Expression(token, Node::Type::Integer) {}

    void setValue(int64_t value)
    {
        _value = value;
    }

    int64_t value() const
    {
        return _value;
    }

    string toString() const override
    {
        return literal();
    }

private:
    int64_t _value;
};


}
