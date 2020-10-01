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
	FunctionCaller() = default;

	FunctionCaller(func_ptr _func, FunctionArgumentsData const& _args) :
		func(std::move(_func)), args(_args) {}

	type call(child const& arguments) const
	{
		FunctionArguments f;
		for (auto const& i : arguments)
			f.unite(i);
		
		return func->calculate(f.superimpose(args));
	}

	operator bool() const
	{
		return func != nullptr;
	}

	const func_ptr get() const
	{
		return func;
	}
private:
	FunctionArgumentsData args;
	func_ptr func = nullptr;
};

class FunctionByName final
{
public:
	FunctionByName() = default;

	void AddFunction(func_ptr func, FunctionArgumentsData const& args)
	{
		if (args.is_template)
		{
			if(temp)
				throw std::exception("This function with template already exists");
			
			temp = FunctionCaller(func, args);
		}

		size_t cnt = args.names.size();

		if (funcs.find(cnt) != funcs.end())
			throw std::exception("This function alredy exists");

		funcs[cnt] = FunctionCaller(func, args);
	}

	type call(child const& arguments) const
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

	const func_ptr get(uint64_t cnt) const
	{
		auto it = funcs.find(cnt);
		if (it != funcs.end())
			return it->second.get();
		
		if (temp_cnt && temp_cnt <= cnt)
			return temp.get();

		return nullptr;
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
		FunctionArgumentsData const& args)
	{
		funcs[std::string(name)].AddFunction(func, args);
	}

	type call(std::string_view name, child const& arguments) const
	{
		return funcs.at(std::string(name)).call(arguments);
	}

	const func_ptr get(std::string const& s, uint64_t cnt) const
	{
		auto it = funcs.find(s);
		if (it != funcs.end())
			return it->second.get(cnt);

		return nullptr;
	}

	auto get(std::string_view s, uint64_t cnt) const
	{
		std::string t(s.data(), s.size());
		return get(t, cnt);
	}
private:
	std::unordered_map<std::string, FunctionByName> funcs;
};