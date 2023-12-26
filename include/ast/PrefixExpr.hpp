#pragma once

#include "basic/Expression.hpp"
#include <sstream>

namespace li
{


class PrefixExpr : public Expr
{
public:
    PrefixExpr(const shared_ptr<Token>& token) :
        Expr(token, Type::Prefix) {}

    string toString() const override
    {
        stringstream stream;
        stream << operatorName << right->toString();
        return stream.str();
    }

public:
    shared_ptr<Expr> right;
    string operatorName;
};


}
