#pragma once

#include "basic/Statement.hpp"

namespace li
{


class Program : public Node
{
public:
    Program(shared_ptr<Token> token) :
        Node(token, Node::Type::Program) {}

    string literal() const override
    {
        string result;
        for (const auto& Stat : statements)
        {
            result += Stat->literal() + "\n";
        }
        return result;
    }

    string toString() const override
    {
        stringstream stream;
        for (const auto& Stat : statements)
        {
            stream << Stat->toString() << "; ";
        }
        return stream.str();
    }

public:
    vector<shared_ptr<Stat>> statements;
};


}
