#pragma once
#include "IFunction.h"

#include <array>

using bin_data = std::array<std::unique_ptr<IFunction>, 2>;

class IBinaryFunction : public IFunction
{
protected:
	bin_data todo;

	void copy(child&& args)
	{
		for (size_t i = 0; i < 2; ++i)
			todo[i] = std::move(args[i]);
	}
};