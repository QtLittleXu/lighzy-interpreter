#pragma once

#include "basic/Expression.hpp"

namespace li
{


class BoolExpr : public Expr
{
public:
    BoolExpr(const shared_ptr<Token>& token) :
        Expr(token, Type::Bool) {}

    string toString() const override
    {
        return value ? "true" : "false";
    }

public:
    bool value;
};


}
