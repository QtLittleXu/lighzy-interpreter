#pragma once

#include "Node.hpp"

namespace li
{


class Statement : public Node
{
public:
    Statement(const shared_ptr<Token>& token, Type type = Node::Type::Statement) : Node(token, type) {}

    virtual string literal() const override
    {
        return _token->literal();
    }
    
    virtual string toString() const override
    {
        return "statement";
    }
};


}
