#pragma once
#include "IFunction.h"
#include "../FunctionHelper.h"

#include <cassert>

class Power final : public IFunction
{
public:
	factory(Power)
protected:
	type calculate(std::vector<type>&& args) const override
	{
		if (args.size() == 1)
			args.insert(args.begin(), std::exp(1));

		assert(args.size() == 2);

		type base = args[0];
		type exp = args[1];

		type res = pow(base, exp);
		return res;
	}
};