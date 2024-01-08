#pragma once

#include "basic/Statement.hpp"
#include "basic/Expression.hpp"
#include "IdentifierExpr.hpp"
#include <memory>
#include <sstream>

namespace li
{


class LetStat : public Stat
{
public:
    LetStat(shared_ptr<Token> token) :
        Stat(token, Type::Let) {}

    string toString() const override
    {
        stringstream stream;
        stream << token->literal << " " << name->toString() << " = " << value->toString();
        return stream.str();
    }

public:
    shared_ptr<IdentifierExpr> name;
    shared_ptr<Expr> value;
};


}
