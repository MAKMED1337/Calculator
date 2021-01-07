#pragma once
#include "IFunction.h"
#include "../FunctionNamespace.h"

class Caller final : public IFunction
{
public:
	Caller() = default;

	Caller(std::string_view s,
		std::shared_ptr<FunctionNamespace> function_namespace, child&& a) :
		name(std::string(s)), f(function_namespace)
	{ build(std::move(a)); }

	factory(Caller)
protected:
	type calculate(std::vector<type>&& args) const override
	{
		return f->call(name, std::move(args));
	}
private:
	std::shared_ptr<FunctionNamespace> f;
	std::string name;
};