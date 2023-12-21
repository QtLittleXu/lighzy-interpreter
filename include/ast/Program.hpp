#pragma once

#include "basic/Statement.hpp"
#include <sstream>
#include <vector>

namespace li
{


class Program : public Node
{
public:
    Program(const shared_ptr<Token>& token) :
        Node(token, Node::Type::Program) {}

    string literal() const override
    {
        string result;
        for (const auto& statement : _statements)
        {
            result += statement->literal() + "\n";
        }
        return result;
    }
    
    void addStatement(const shared_ptr<Statement>& statement)
    {
        _statements.push_back(statement);
    }

    const auto& statements() const
    {
        return _statements;
    }

    string toString() const override
    {
        stringstream stream;
        for (const auto& statement : _statements)
        {
            stream << statement->toString() << "; ";
        }
        return stream.str();
    }

private:
    vector<shared_ptr<Statement>> _statements;
};


}
