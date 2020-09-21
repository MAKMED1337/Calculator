#pragma once
#include "IFunction.h"

class Plus final : public IFunction
{
public:
	void build(child&& args) override
	{
		todo = std::move(args);
	}
	
	type calculate(values const& args) const override
	{
		type res = 0;

		for (auto const& t : todo)
			res += t->calculate(args);
		
		return res;
	}

	factory(Plus)
private:
	child todo;
};