#pragma once

#include "basic/Statement.hpp"

namespace li
{


class BlockStat : public Stat
{
public:
	BlockStat(const shared_ptr<Token>& token) :
		Stat(token, Type::Block) {}

	string toString() const override
	{
		stringstream buffer;
		buffer << "{ ";
		for (const auto& stat : statements)
		{
			buffer << stat->toString() << "; ";
		}
		buffer << " }";
		return buffer.str();
	}

public:
	vector<shared_ptr<Stat>> statements;
};


}