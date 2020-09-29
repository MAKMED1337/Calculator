#include "IFunction.h"

FunctionArguments::FunctionArguments(func_ptr f)
{
	push_back(f);
}

void FunctionArguments::operator= (func_ptr f)
{
	push_back(f);
}

void FunctionArguments::unite(func_ptr f)
{
	push_back(f);
}

void unite(FunctionArguments const& fa)
{
	for (auto i : fa)
		unite(i);
}

std::unordered_map<std::string, FunctionArguments>
	FunctionArguments::superimpose(FunctionArgumentsData const& fad) const
{
	std::unordered_map<std::string, FunctionArguments> res;
	size_t cnt = std::min(fad.names.size(), res.size());

	for (size_t i = 0; i < fad.names.size(); ++i)
		res[fad.names[i]].unite(at(i));

	if (fad.is_template)
		for (size_t i = cnt; i < size(); ++i)
		{
			res["..."].unite(at(i));
			++cnt;
		}

	if (cnt < size())
		throw std::exception("Not enough arguments");

	return res;
}