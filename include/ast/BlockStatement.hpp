#pragma once

#include "basic/Statement.hpp"

namespace li
{


class BlockStatement : public Statement
{
public:
	BlockStatement(const shared_ptr<Token>& token) :
		Statement(token, Node::Type::Block) {}

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
		stringstream buffer;
		buffer << "{ ";
		for (const auto& statement : _statements)
		{
			buffer << statement->toString() << "; ";
		}
		buffer << " }";
		return buffer.str();
	}

private:
	vector<shared_ptr<Statement>> _statements;
};


}