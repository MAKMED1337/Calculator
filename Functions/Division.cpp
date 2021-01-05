#pragma once
#include "IFunction.h"

#include <cassert>

class Division final : public IFunction
{
public:
	factory(Division)
protected:
	type calculate(std::vector<type>&& args) const override
	{
		assert(args.size() == 2);

		type res = args[0] / args[1];
		return res;
	}
};