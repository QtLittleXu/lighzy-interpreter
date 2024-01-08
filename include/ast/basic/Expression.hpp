#pragma once

#include "Node.hpp"

namespace li
{


class Expr : public Node
{
public:
    Expr(shared_ptr<Token> token, Type type = Type::Expr) : Node(token, type) {}

    virtual string literal() const override
    {
        return token->literal;
    }
    
    virtual string toString() const override
    {
        return "Expr";
    }
};


}
