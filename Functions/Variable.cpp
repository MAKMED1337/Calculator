#pragma once
#include "IFunction.h"

#include <ctype.h>

class Variable final : public IFunction
{
public:
	Variable(std::string_view s)
	{
		for (char c : s)
			if (!isalpha(c))
				throw std::exception("wrong variable name");

		name = s;
	}

	void build(child&& args) override {}

	type calculate(values args) const override
	{
		return args.at(name)->calculate();
		static_assert(0);
	}

	factory(Variable)
private:
	std::string name;
};