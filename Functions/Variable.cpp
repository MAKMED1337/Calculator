#pragma once
#include "IFunction.h"

#include <ctype.h>

static bool is_valid(std::string_view s)
{
	if (s.size() == 0)
		return false;

	for (char i : s)
		if (!isalpha(i))
			return false;
	
	return true;
}

class Variable final : public IFunction
{
public:
	Variable() = default;

	Variable(std::string_view s)
	{
		if(!is_valid(s))
			throw std::exception("wrong variable name");

		name = s;
	}

	void build(child&& args) override {}

	type calculate(values&& args, std::vector<type>&& templates) const override
	{
		return args.at(name);
	}

	factory(Variable)
protected:
	type calculate(std::vector<type>&& args) const override { return NAN; } //useless
private:
	std::string name;
};