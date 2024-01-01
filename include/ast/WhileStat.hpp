#pragma once

#include "basic/Statement.hpp"
#include "basic/Expression.hpp"
#include "BlockStat.hpp"

namespace li
{


class WhileStat : public Stat
{
public:
    WhileStat(const shared_ptr<Token>& token) :
        Stat(token, Type::While) {}

    string toString() const override
    {
		stringstream buffer;
		buffer << "while (" << condition->toString() << ")" << body->toString();
		return buffer.str();
    }

public:
	shared_ptr<Expr> condition;
	shared_ptr<BlockStat> body;
};


}
