#pragma once
#include "IFunction.h"
#include "IBinaryFunction.h"

#include <assert.h>

class Minus final  : public IBinaryFunction
{
public:
	void build(child&& args) override
	{
		assert(args.size() == 2);

		copy(std::move(args));
	}

	type calculate(values const& args) const override
	{
		type res = 0;

		res += todo[0]->calculate(args);
		res -= todo[1]->calculate(args);
		
		return res;
	}

	factory(Minus)
};