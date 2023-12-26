#pragma once

#include "lexer/Token.h"

namespace li
{


class Node
{
public:
    enum class Type
    {
        Program, Expr, Stat,
        Let, Return, Arguments, Exprs, Block,
        Call, Function, ExprStat, Identifier,
        Integer, Bool, Infix, Prefix, If, String, Assign
    };

public:
    Node(const shared_ptr<Token>& token, Type type) : token(token), type(type) {}
    virtual string literal() const = 0;
    virtual string toString() const = 0;

public:
    shared_ptr<Token> token;
    Type type;
};


}