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

	FunctionCaller(cfunc_ptr _func, FunctionArguments&& _args)
	{
		SetFunction(_func, std::move(_args));
	}

	void SetFunction(cfunc_ptr _func, FunctionArguments&& _args)
	{
		func = std::move(_func);
		args = std::move(_args);
	}

	bool can_call(size_t size) const
	{
		return args.can_call(size);
	}

	type call(std::vector<type>&& arguments) const
	{
		auto x = args.superimpose(std::move(arguments));
		return func->calculate(std::move(x.first), std::move(x.second));
	}

	cfunc_ptr get() const
	{
		return func;
	}

	operator bool() const
	{
		return func != nullptr;
	}
private:
	FunctionArguments args;
	cfunc_ptr func = nullptr;
};

class FunctionByName final
{
public:
	FunctionByName() = default;

	void AddFunction(cfunc_ptr func, FunctionArguments&& args)
	{
		if (args.is_template())
		{
			if(temp)
				throw std::exception("Function with template already exists");
			
			temp = FunctionCaller(func, std::move(args));
			return;
		}

		size_t cnt = args.size();
		if (funcs.find(cnt) != funcs.end())
			throw std::exception("This function alredy exists");

		funcs[cnt] = FunctionCaller(func, std::move(args));
	}

	type call(std::vector<type>&& arguments) const
	{
		auto it = funcs.find(arguments.size());
		if (it == funcs.end())
		{
			if (temp)
				return temp.call(std::move(arguments));

			throw std::exception("No function with this argument count");
		}

		return it->second.call(std::move(arguments));
	}

	const FunctionCaller* get(size_t size) const
	{
		auto it = funcs.find(size);
		if (it != funcs.end())
			return &it->second;

		if (temp && temp.can_call(size))
			return &temp;

		return nullptr;
	}

	const FunctionCaller& get_or_create(size_t size)
	{
		return funcs[size];
	}
private:
	std::map<size_t, FunctionCaller> funcs;
	FunctionCaller temp;
	uint64_t temp_cnt = 0;
};

class FunctionNamespace final
{
public:
	FunctionNamespace() = default;

	void AddFunction(std::string&& name, cfunc_ptr func,
		FunctionArguments&& args)
	{
		funcs[std::move(name)].AddFunction(func, std::move(args));
	}

	type call(std::string const& name, std::vector<type>&& arguments) const
	{
		auto it = funcs.find(name);
		if (it == funcs.end())
			throw std::exception("No function wtih this name");

		return it->second.call(std::move(arguments));
	}

	const FunctionByName* get(std::string const& name) const
	{
		auto it = funcs.find(name);
		if (it == funcs.end())
			return nullptr;
		
		return &it->second;
	}

	const FunctionByName& get_or_create(std::string const& name)
	{
		return funcs[name];
	}
protected:
	std::unordered_map<std::string, FunctionByName> funcs;
};