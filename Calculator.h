#pragma once
#include "Functions/IFunction.h"

#include "Functions/Caller.cpp"
#include "Functions/Constant.cpp"
#include "Functions/Variable.cpp"

#include "FunctionNamespace.h"

#include "FunctionHelper.h"

#include <iostream>
#include <array>

constexpr int64_t alphabet_length = 127,
	not_operator = INT64_MIN;

class Calculator final
{
public:
	struct binary_operator final
	{
		int64_t priority = not_operator, next_priority = not_operator;
		std::string name;
	};

	/*
	dynamic namespace - finds, then call function when calculate called
	static namespace - it inserts function on parsing

	static namespace uses when function what we calls can be find in
	static namespace and function what we call didnt use templates
	otherwise use dynamic namespace
	*/
	Calculator(std::string_view s,
		namespace_ptr _static_func,
		namespace_ptr _dynamic_func,
		std::array<binary_operator, alphabet_length> const& _bin_op) :
			static_func(_static_func), dynamic_func(_dynamic_func), bin_op(_bin_op)
	{
		result = parse_binary(s).first;
		static_func.reset();
	}

	type calculate(values&& args = {}, std::vector<type>&& templates = {}) const;

	cfunc_ptr get_result() const
	{
		return result;
	}
private:
	namespace_ptr static_func, dynamic_func;
	std::array<binary_operator, alphabet_length> bin_op;

	cfunc_ptr result;


	cfunc_ptr get_function(std::string_view s, child&& args) const;

	cfunc_ptr parse_primitive(std::string_view s) const;

	size_t parse_brackets(std::string_view s) const;
	size_t parse_logical_unit(std::string_view s) const;

	child parse_arguments(std::string_view s) const;

	cfunc_ptr parse_unit(std::string_view s) const;

	std::pair<cfunc_ptr, std::string_view>
		parse_binary(std::string_view s, int64_t priority = not_operator + 1) const;
};