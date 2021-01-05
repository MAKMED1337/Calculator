#pragma once
#include "IFunction.h"

class Constant final : public IFunction
{
public:
	Constant() = default;

	Constant(type x) : value(x) {}

	void build(child&& args) override {}

	factory(Constant)
protected:
	type calculate(std::vector<type>&& args) const override
	{
		return value;
	}
private:
	type value = 0;
};