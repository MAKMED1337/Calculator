#pragma once
#include "IFunction.h"
#include "IBinaryFunction.h"
#include "FunctionHelper.h"

#include <cassert>

class Power final : public IBinaryFunction
{
public:
	void build(child&& args) override
	{
		assert(args.size() >= 1 && args.size() <= 2);

		if (args.size() == 1)
		{
			todo[0] = get_const(std::exp(1));
			todo[1] = std::move(args[0]);

			return;
		}

		copy(std::move(args));
	}

	type calculate(values args) const override
	{
		type base = todo[0]->calculate(args);
		type exp = todo[1]->calculate(args);
		
		return pow(base, exp);
	}

	factory(Power)
};