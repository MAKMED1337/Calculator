#pragma once
#include "IFunction.h"

class Plus final : public IFunction
{
public:
	void build(child&& args) override
	{
		todo = std::move(args);
	}

	factory(Plus)
protected:
	type calculate(std::vector<type>&& args) const override
	{
		type res = 0;

		for (auto const& t : args)
			res += t;

		return res;
	}
};