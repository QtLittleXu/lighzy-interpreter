#pragma once

#include "lexer/Token.h"

namespace li
{


class Node
{
public:
    enum class Type
    {
        Program, Expression, Statement,
        Let, Return, Arguments, Expressions, Block,
        Call, Function, ExpressionStatement, Identifier,
        Integer, Bool, Infix, Prefix, If, String, Assign
    };

public:
    Node(const shared_ptr<Token>& token, Type type) : _token(token), _type(type) {}
    virtual string literal() const = 0;
    virtual string toString() const = 0;

public:
    Type type() const
    {
        return _type;
    }

    void setToken(const shared_ptr<Token>& token)
    {
        _token = token;
    }

    const auto& token() const
    {
        return _token;
    }

protected:
    shared_ptr<Token> _token;
    Type _type;
};


}