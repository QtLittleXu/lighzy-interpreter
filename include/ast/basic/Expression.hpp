#pragma once

#include "Node.hpp"

namespace li
{


class Expression : public Node
{
public:
    Expression(const shared_ptr<Token>& token, Type type = Node::Type::Expression) : Node(token, type) {}

    virtual string literal() const override
    {
        return _token->literal();
    }
    
    virtual string toString() const override
    {
        return "expression";
    }
};


}
