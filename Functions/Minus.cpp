#pragma once
#include "IFunction.h"

#include <cassert>

class Minus final : public IFunction
{
public:
	factory(Minus)
protected:
	type calculate(std::vector<type>&& args) const override
	{
		assert(args.size() == 2);

		type res = args[0] - args[1];
		return res;
	}
};