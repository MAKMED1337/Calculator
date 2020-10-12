#include "FunctionArguments.h"

values FunctionArguments::superimpose(std::vector<type> const& args) const
{
	values res;
	size_t cnt = std::min(args.size(), size());

	for (size_t i = 0; i < args.size(); ++i)
		res[names[i]] = args[i];

	if (is_template())
		for (size_t i = cnt; i < names.size(); ++i)
		{
			res["..."] = args[i];
			++cnt;
		}

	if (cnt < args.size())
		throw std::exception("Not enough arguments");

	return res;
}