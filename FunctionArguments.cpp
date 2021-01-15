#include "FunctionArguments.h"

FunctionArguments::FunctionArguments(std::vector<std::string>&& _names)
{
	names = std::move(_names);
	for (size_t i = 0; i < names.size(); ++i)
		if (names[i] == "...")
		{
			if (is_template())
				throw std::exception("2 or more templates in function");
			
			pos = i;
		}
}

bool FunctionArguments::can_call(size_t sz) const
{
	if (is_template())
		return size() >= sz;

	return size() == sz;
}

std::pair<values, std::vector<type>>
	FunctionArguments::superimpose(std::vector<type>&& args) const
{
	values vals;
	std::vector<type> templates;

	if (!is_template()) //always valid
	{
		if (names.size() != args.size()) //doesn't need but i want
			throw std::exception("Arguments size didn't match");

		for (size_t i = 0; i < names.size(); ++i)
			vals[names[i]] = args[i];
	}
	else
	{
		if (args.size() < size())
			throw std::exception("Not enough arguments");

		for (size_t i = 0; i < pos; ++i)
			vals[names[i]] = args[i];

		//args.size() - 1 = t + names.size() - 1
		int64_t t = args.size() - names.size();
		for (size_t i = names.size() - 1; i > pos; --i)
			vals[names[i]] = args[t + i];

		size_t last = t + pos;
		for (size_t i = pos; i <= last; ++i)
			templates.push_back(args[i]);
	}

	return { vals, templates };
}