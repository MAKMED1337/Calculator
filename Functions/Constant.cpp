#pragma once
#include "IFunction.h"

class Constant final : public IFunction
{
public:
	Constant() = default;

	Constant(type x) : value(x) {}

	void build(child&& args) override {}

	type calculate(values const& args) const override
	{
		return value;
	}

	factory(Constant)
private:
	type value;
};