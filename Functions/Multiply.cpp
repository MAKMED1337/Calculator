#pragma once
#include "IFunction.h"

class Multiply final  : public IFunction
{
public:
	void build(child&& args) override
	{
		todo = std::move(args);
	}

	type calculate(values args) const override
	{
		type res = 1;

		for (auto const& t : todo)
			res *= t->calculate(args);

		return res;
	}

	factory(Multiply)
private:
	child todo;
};