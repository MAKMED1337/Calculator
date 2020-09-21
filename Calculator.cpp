#include "Calculator.h"

std::unique_ptr<IFunction> Calculator::parse_primitive(std::string_view s) const
{
	auto t = is_number<type>(s);

	if (t)
		return std::make_unique<Variable>(s);
	
	return std::make_unique<Constant>(*t);
}

size_t Calculator::parse_brackets(std::string_view s) const
{
	int64_t cnt = 0;

	if (s.empty() || s[0] != '(')
		return 0;

	for (size_t i = 0; i < s.size(); ++i)
		switch (s[i])
		{
		case '(':
			++cnt;
			break;
		case ')':
			--cnt;
			if (cnt < 0)
				throw std::exception("wrong brackets");

			if (cnt == 0)
				return i;

			break;
		}

	throw std::exception("wrong brackets");
}

size_t Calculator::parse_logical_unit(std::string_view s) const
{
	size_t t = 0;

	while (t < s.size() && bin_op[s[t]].priority == not_operator)
		t += parse_brackets(s.substr(t)) + 1;

	return t;
}

child Calculator::parse_arguments(std::string_view s) const
{
	//std::cerr << "args: " << s << "\n";
	child res;

	while (!s.empty())
	{
		size_t ind = 0;
		while (ind < s.size() && s[ind] != ',')
			ind += parse_brackets(s.substr(ind)) + 1;

		res.push_back(parse_binary(s.substr(0, ind)).first);

		s = s.substr(std::min(ind + 1, s.size()));
	}

	return res;
}

std::unique_ptr<IFunction> Calculator::parse_unit(std::string_view s) const
{
	s = trim(s);
	//std::cerr << "unit: " << s << "\n";

	auto it = s.find('(');
	if (it == std::string_view::npos)
		return parse_primitive(s);

	if (s.back() != ')')
		throw std::exception("wrong brackets");

	if (it == 0)
		return parse_binary(s.substr(1, s.size() - 2)).first;

	auto x = func.find(s.substr(0, it));
	if (!it && x == func.end())
		throw std::exception("no such function");

	child args = parse_arguments(s.substr(it + 1, s.size() - it - 2));

	return x->second->create(std::move(args));
}

std::pair<std::unique_ptr<IFunction>, std::string_view>
	Calculator::parse_binary(std::string_view s, int64_t priority) const
{
	s = ltrim(s);
	//std::cerr << priority << "\t\t" << s << "\n";

	size_t ind = parse_logical_unit(s);
	if (ind == s.size())
		return { parse_unit(s), "" };

	std::unique_ptr<IFunction> f = parse_binary(s.substr(0, ind), not_operator + 1).first;
	//if (s[0] == '-')
	//	f = std::make_unique<Primitive>(0);

	s = s.substr(ind);

	while (!s.empty())
	{
		auto priority_ = bin_op[s[0]].priority;
		if (priority_ < priority)
			break;

		auto x = parse_binary(s.substr(1), bin_op[s[0]].next_priority);

		child args(2);
		args[0] = std::move(f);
		args[1] = std::move(x.first);

		f = bin_op[s[0]].func->create(std::move(args));

		s = x.second;
	}

	return { std::move(f), s };
}

type Calculator::calculate(values v) const
{
	return result->calculate(v);
}