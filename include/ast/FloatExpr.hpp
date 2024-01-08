#pragma once

#include "basic/Expression.hpp"
#include <string>

namespace li
{


class FloatExpr : public Expr
{
public:
    FloatExpr(shared_ptr<Token> token) :
        Expr(token, Type::Float) {}

    string toString() const override
    {
        stringstream buffer;
        buffer << value;
        return buffer.str();
    }

public:
    double value;
};


}
