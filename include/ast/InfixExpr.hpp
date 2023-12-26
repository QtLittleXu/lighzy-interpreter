#pragma once

#include "basic/Expression.hpp"
#include <string>

namespace li
{


class InfixExpr : public Expr
{
public:
    InfixExpr(const shared_ptr<Token>& token) :
        Expr(token, Type::Infix) {}

    string toString() const override
    {
        stringstream stream;
        stream << "("  << left->toString() << " " << operatorName << " " << right->toString() << ")";
        return stream.str();
    }

public:
    shared_ptr<Expr> left;
    shared_ptr<Expr> right;
    string operatorName;
};


}
