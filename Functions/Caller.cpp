#pragma once
#include "IFunction.h"
#include "../FunctionNamespace.h"

class Caller final : public IFunction
{
public:
	Caller() = default;

	Caller(FunctionCaller const& _f, child&& a) :
		f(_f) { todo = std::move(a); }

	void build(child&& args) override {}

	factory(Caller)
protected:
	type calculate(std::vector<type>&& args) const override
	{
		return f.call(std::move(args));
	}
private:
	FunctionCaller f;
};