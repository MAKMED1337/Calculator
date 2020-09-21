#pragma once
#include "Functions/IFunction.h"

#include <vector>
#include <string>
#include <unordered_map>

struct FunctionArgumentsData
{
	std::vector<std::string> names;
	bool is_template = false;
};

class FunctionArguments
{
public:
	void unite(func_ptr f)
	{
		v.push_back(f);
	}

	void unite(FunctionArguments const& fa)
	{
		for (auto& i : fa.v)
			unite(i);
	}

	std::unordered_map<std::string, FunctionArguments>
		superimpose(FunctionArgumentsData const& fad)
	{
		std::unordered_map<std::string, FunctionArguments> res;
		size_t cnt = min(fad.names.size(), res.size());

		for (size_t i = 0; i < fad.names.size(); ++i)
			res[fad.names[i]] = v[i];

		if (fad.is_template)
			for (size_t i = cnt; i < v.size(); ++i)
			{
				res["..."].unite(v[i]);
				++cnt;
			}
		
		if (cnt < v.size())
			throw std::exception("Not enough arguments");
		
		return res;
	}
private:
	std::vector<std::shared_ptr<IFunction>> v;
};