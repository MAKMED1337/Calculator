#pragma once
#include "IFunction.h"
#include "../FunctionHelper.h"

#include <cassert>

class Root final : public IFunction //same pow(a, 1 / b)
{
public:
	factory(Root)
protected:
	type calculate(std::vector<type>&& args) const override
	{
		if (args.size() == 1)
			args.emplace_back(2);

		assert(args.size() == 2);

		type base = args[0];
		type exp = args[1];
		
		type res = pow(base, 1.0L / exp);
		return res;
	}
};