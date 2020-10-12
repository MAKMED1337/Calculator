#pragma once
#include "Functions/IFunction.h"

#include <string>
#include <vector>

class FunctionArguments
{
public:
	FunctionArguments() = default;

	FunctionArguments(std::vector<std::string> _names, bool _have_template)
		: names(_names), have_template(_have_template) {}

	values superimpose(std::vector<type> const& args) const;

	bool is_template() const { return have_template; }
	size_t size() const { return names.size(); }
private:
	std::vector<std::string> names;
	bool have_template = false;
};