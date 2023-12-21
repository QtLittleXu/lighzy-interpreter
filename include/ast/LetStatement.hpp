#pragma once

#include "basic/Statement.hpp"
#include "basic/Expression.hpp"
#include "IdentifierExpression.hpp"
#include <memory>
#include <sstream>

namespace li
{


class LetStatement : public Statement
{
public:
    LetStatement(const shared_ptr<Token>& token) :
        Statement(token, Node::Type::Let) {}

    void setValue(const shared_ptr<Expression>& value)
    {
        _value = value;
    }

    const auto& value() const
    {
        return _value;
    }

    void setName(const shared_ptr<IdentifierExpression>& name)
    {
        _name = name;
    }

    const auto& name() const
    {
        return _name;
    }

    string toString() const override
    {
        stringstream stream;
        stream << _token->literal() << " " << _name->toString() << " = " << _value->toString();
        return stream.str();
    }

private:
    shared_ptr<IdentifierExpression> _name;
    shared_ptr<Expression> _value;
};


}
