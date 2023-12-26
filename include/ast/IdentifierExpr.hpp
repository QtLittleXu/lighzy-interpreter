#pragma once

#include "basic/Expression.hpp"

namespace li
{


class IdentifierExpr : public Expr
{
public:
    IdentifierExpr(const shared_ptr<Token>& token) :
        Expr(token, Type::Identifier) {}

    string toString() const override
    {
        return value;
    }

public:
    string value;
};


}
