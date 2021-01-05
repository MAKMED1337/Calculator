#include "FunctionArguments.h"

std::pair<values, std::vector<type>>
	FunctionArguments::superimpose(std::vector<type> const& args) const
{
	std::pair<values, std::vector<type>> res;
	size_t cnt = std::min(args.size(), size());

	for (size_t i = 0; i < args.size(); ++i)
		res.first[names[i]] = { args[i] };

	if (is_template())
		for (size_t i = cnt; i < names.size(); ++i)
			res.second.push_back(args[i]);

	if (cnt < args.size())
		throw std::exception("Not enough arguments");

	return res;
}