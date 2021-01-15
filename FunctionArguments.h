#pragma once
#include "Functions/IFunction.h"

#include <cassert>
#include <string>
#include <vector>

class FunctionArguments
{
public:
	FunctionArguments() = default;

	FunctionArguments(std::vector<std::string>&& _names);

	std::pair<values, std::vector<type>> superimpose(std::vector<type>&& args) const;

	bool can_call(size_t size) const;

	bool is_template() const { return pos != -1; }
	size_t size() const { return names.size() - is_template(); }
private:
	std::vector<std::string> names;
	int64_t pos = -1;
};