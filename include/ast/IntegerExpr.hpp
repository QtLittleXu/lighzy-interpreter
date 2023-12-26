#pragma once

#include "basic/Expression.hpp"
#include <string>

namespace li
{


class IntegerExpr : public Expr
{
public:
    IntegerExpr(const shared_ptr<Token>& token) :
        Expr(token, Type::Integer) {}

    string toString() const override
    {
        return to_string(value);
    }

public:
    int64_t value;
};


}
