#pragma once
#include "IFunction.h"

#include "../FunctionHelper.h"

#include <cassert>

class Logarithm final : public IFunction
{
public:
	factory(Logarithm)
protected:
	type calculate(std::vector<type>&& args) const override
	{
		if (args.size() == 1)
			args.insert(args.begin(), std::exp(1));

		assert(args.size() == 2);

		type base = args[0];
		type value = args[1];

		type res = log(base, value);
		return res;
	}
};