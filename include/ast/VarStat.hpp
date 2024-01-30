#pragma once

#include "basic/Statement.hpp"
#include "basic/Expression.hpp"
#include "IdentifierExpr.hpp"

namespace li
{


class VarStat : public Stat
{
public:
    VarStat(shared_ptr<Token> token) :
        Stat(token, Type::Var) {}

    string toString() const override
    {
        stringstream stream;
        stream << "var " << name->toString() << " = " << value->toString();
        return stream.str();
    }

public:
    shared_ptr<IdentifierExpr> name;
    shared_ptr<Expr> value;
};


}
