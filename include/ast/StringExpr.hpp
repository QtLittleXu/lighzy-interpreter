#pragma once

#include "basic/Expression.hpp"

namespace li
{


class StringExpr : public Expr
{
public:
    StringExpr(shared_ptr<Token> token) :
        Expr(token, Node::Type::String) {}

    string toString() const override
    {
        return value;
    }

public:
    string value;
};


}
