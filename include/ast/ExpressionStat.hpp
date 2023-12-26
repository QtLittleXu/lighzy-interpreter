#pragma once

#include "basic/Statement.hpp"
#include "basic/Expression.hpp"

namespace li
{


class ExpressionStat : public Stat
{
public:
    ExpressionStat(const shared_ptr<Token>& token) :
        Stat(token, Type::ExprStat) {}

    string toString() const override
    {
        return expression->toString();
    }

public:
    shared_ptr<Expr> expression;
};


}
