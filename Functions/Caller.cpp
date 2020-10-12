#pragma once
#include "IFunction.h"
#include "../FunctionNamespace.h"

class Caller final : public IFunction
{
public:
	Caller() = default;

	Caller(FunctionCaller const& _f, child&& a) :
		f(_f), c(a) {}

	void build(child&& args) override {}

	type calculate(values const& arguments) const override
	{
		size_t n = c.size();
		std::vector<type> args(n);
		for (size_t i = 0; i < n; ++i)
			args[i] = c[i]->calculate(arguments);

		return f.call(args);
	}

	factory(Caller)
private:
	FunctionCaller f;
	child c;
};