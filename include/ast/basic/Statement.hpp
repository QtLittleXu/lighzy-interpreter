#pragma once

#include "Node.hpp"

namespace li
{


class Stat : public Node
{
public:
    Stat(const shared_ptr<Token>& token, Type type = Type::Stat) : Node(token, type) {}

    virtual string literal() const override
    {
        return token->literal;
    }
    
    virtual string toString() const override
    {
        return "Stat";
    }
};


}
