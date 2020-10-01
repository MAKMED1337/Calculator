#pragma once
#include "Functions/IFunction.h"
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
		func_ptr func;
	};

	Calculator(std::string_view s,
		FunctionNamespace const& _func,
		std::array<binary_operator, alphabet_length> const& _bin_op) :
			func(_func), bin_op(_bin_op)
	{
		result = parse_binary(s).first;
	}

	type calculate(values const& args) const;

	const func_ptr get_func() const
	{
		return result;
	}
private:
	FunctionNamespace func;
	std::array<binary_operator, alphabet_length> bin_op;

	func_ptr result;


	func_ptr parse_primitive(std::string_view s) const;

	size_t parse_brackets(std::string_view s) const;
	size_t parse_logical_unit(std::string_view s) const;

	child parse_arguments(std::string_view s) const;

	func_ptr parse_unit(std::string_view s) const;

	std::pair<func_ptr, std::string_view>
		parse_binary(std::string_view s, int64_t priority = not_operator + 1) const;
};