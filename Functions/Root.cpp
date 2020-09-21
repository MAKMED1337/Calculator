#pragma once
#include "IFunction.h"
#include "IBinaryFunction.h"
#include "FunctionHelper.h"

#include <assert.h>

class Root final : public IBinaryFunction //same pow(a, 1 / b)
{
public:
	void build(child&& args) override
	{
		assert(args.size() >= 1 && args.size() <= 2);

		todo[0] = std::move(args[0]);

		if (args.size() == 1)
			todo[1] = std::move(get_const(2));
		else
			todo[1] = std::move(args[1]);
	}

	type calculate(values args) const override
	{
		type base = todo[0]->calculate(args);
		type exp = todo[1]->calculate(args);

		return pow(base, 1.0 / exp);
	}

	factory(Root)
};