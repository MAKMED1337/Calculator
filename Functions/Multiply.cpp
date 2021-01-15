#pragma once
#include "IFunction.h"

class Multiply final : public IFunction
{
public:
	factory(Multiply)
protected:
	type calculate(std::vector<type>&& args) const override
	{
		type res = 1;

		for (auto const& t : args)
			res *= t;

		return res;
	}
private:
	child todo;
};