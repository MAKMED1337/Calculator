#pragma once
#include "Functions/IFunction.h"
#include "FunctionArguments.h"

#include <vector>
#include <string>
#include <string_view>
#include <memory>
#include <map>

class FunctionCaller final
{
public:
	FunctionCaller() = default;

	FunctionCaller(func_ptr _func, FunctionArguments const& _args) :
		func(std::move(_func)), args(_args) {}

	type call(std::vector<type> const& arguments) const
	{
		return func->calculate(args.superimpose(arguments));
	}

	operator bool() const
	{
		return func != nullptr;
	}
private:
	FunctionArguments args;
	func_ptr func = nullptr;
};

class FunctionByName final
{
public:
	FunctionByName() = default;

	void AddFunction(func_ptr func, FunctionArguments const& args)
	{
		if (args.is_template())
		{
			if(temp)
				throw std::exception("This function with template already exists");
			
			temp = FunctionCaller(func, args);
		}

		size_t cnt = args.size();

		if (funcs.find(cnt) != funcs.end())
			throw std::exception("This function alredy exists");

		funcs[cnt] = FunctionCaller(func, args);
	}

	type call(std::vector<type> const& arguments) const
	{
		auto it = funcs.find(arguments.size());
		if (it == funcs.end())
		{
			if (temp)
				return temp.call(arguments);

			throw std::exception("No such function");
		}

		return it->second.call(arguments);
	}

	FunctionCaller get(uint64_t cnt) const
	{
		auto it = funcs.find(cnt);
		if (it != funcs.end())
			return it->second;
		
		if (temp_cnt && temp_cnt <= cnt)
			return temp;

		throw std::exception("No such fucntion");
	}
private:
	std::map<size_t, FunctionCaller> funcs;
	FunctionCaller temp;
	uint64_t temp_cnt = 0;
};

class FunctionNamespace final
{
public:
	void AddFunction(std::string_view name, func_ptr func,
		FunctionArguments const& args)
	{
		funcs[std::string(name)].AddFunction(func, args);
	}

	type call(std::string_view name, std::vector<type> const& arguments) const
	{
		return funcs.at(std::string(name)).call(arguments);
	}

	FunctionCaller get(std::string const& s, uint64_t cnt) const
	{
		auto it = funcs.find(s);
		if (it != funcs.end())
			return it->second.get(cnt);

		throw std::exception("No such fucntion");
	}

	auto get(std::string_view s, uint64_t cnt) const
	{
		std::string t(s.data(), s.size());
		return get(t, cnt);
	}
private:
	std::unordered_map<std::string, FunctionByName> funcs;
};