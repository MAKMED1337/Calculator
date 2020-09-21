#pragma once
#include "Functions/IFunction.h"

#include <vector>
#include <string>
#include <string_view>
#include <memory>
#include <map>

class FunctionCaller final
{
public:
	FunctionCaller(std::unique_ptr<IFunction>&& _func,
		std::vector<std::string>&& _args) :
		func(std::move(_func)), args(_args) {}

	type call(child&& arguments) const
	{
		if (arguments.size() != args.size())
			throw std::exception("Not enought arguments");

		values m;
		for (size_t i = 0; i < args.size(); ++i)
			m[args[i]] = std::move(arguments[i]);

		return func->calculate(m);
	}
private:
	std::vector<std::string> args;
	std::unique_ptr<IFunction> func;
};

class FunctionByName final
{
public:
	void AddFunction(std::unique_ptr<IFunction>&& func,
		std::vector<std::string>&& args)
	{
		size_t cnt = args.size();

		if (funcs.find(cnt) != funcs.end())
			throw std::exception("This function alredy exists");

		funcs[cnt] = FunctionCaller(std::move(func), std::move(args));
	}

	type call(child&& arguments, values&& args) const
	{
		auto it = funcs.find(arguments.size());
		if (it == funcs.end())
			throw std::exception("No such function");

		return it->second.call(std::move(arguments));
	}
private:
	std::map<size_t, FunctionCaller> funcs;
};

class FunctionNamespace final
{
public:
	void AddFunction(std::string_view name,
		std::unique_ptr<IFunction>&& func,
		std::vector<std::string>&& args)
	{
		funcs[std::string(name)].AddFunction(
			std::move(func), std::move(args));
	}

	type call(std::string_view name,
		child&& arguments,
		values&& args) const
	{
		return funcs.at(std::string(name)).call(
			std::move(arguments), std::move(args));
	}
private:
std::unordered_map<std::string, FunctionByName> funcs;
};