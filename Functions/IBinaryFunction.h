#pragma once
#include "IFunction.h"

#include <array>
#include <cassert>

using bin_data = std::array<func_ptr, 2>;

class IBinaryFunction : public IFunction
{
protected:
	bin_data todo;

	void copy(child&& args)
	{
		assert(args.size() <= 2);

		for (size_t i = 0; i < 2; ++i)
			todo[i] = std::move(args[i]);
	}
};