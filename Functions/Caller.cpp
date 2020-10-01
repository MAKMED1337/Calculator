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

	type calculate(values const& args) const override
	{
		return f.call(x);
	}

	factory(Caller)
private:
	FunctionCaller f;
	child c;
};