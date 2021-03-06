#include "Calculator.h"

cfunc_ptr Calculator::get_function(std::string_view s, child&& args) const
{
	bool temp = false;
	for (auto const& i : args)
		if (!i)
			temp = true;
	
	auto x = static_func;
	if (!temp && x)
	{
		auto y = x->get(std::string(s));
		if (y)
		{
			auto z = y->get(args.size());
			auto f = z->get();

			return f->create(std::move(args));
		}
	}

	return std::make_shared<const Caller>(s, dynamic_func, std::move(args));
}

cfunc_ptr Calculator::parse_primitive(std::string_view s) const
{
	if (s == "...")
		return nullptr;

	auto t = is_number<type>(s);

	if (t)
		return get_const(*t);
		
	return std::make_shared<Variable>(s);
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

cfunc_ptr Calculator::parse_unit(std::string_view s) const
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

	child args = parse_arguments(s.substr(it + 1, s.size() - it - 2));
	return get_function(s.substr(0, it), std::move(args));
}

std::pair<cfunc_ptr, std::string_view>
	Calculator::parse_binary(std::string_view s, int64_t priority) const
{
	s = ltrim(s);
	//std::cerr << priority << "\t\t" << s << "\n";

	size_t ind = parse_logical_unit(s);
	if (ind == s.size())
		return { parse_unit(s), "" };

	cfunc_ptr f;
	bool minus = false;

	if (s[0] == '-') //bug fix
	{
		minus = true;
		f = get_const(0);
	}
	else
		f = parse_binary(s.substr(0, ind), not_operator + 1).first;

	s = s.substr(ind);

	while (!s.empty())
	{
		auto priority_ = bin_op[s[0]].priority;
		if (priority_ < priority)
			if (minus)
				minus = false;
			else
				break;

		auto x = parse_binary(s.substr(1), bin_op[s[0]].next_priority);

		child args(2);
		args[0] = std::move(f);
		args[1] = std::move(x.first);

		f = get_function(bin_op[s[0]].name, std::move(args));

		s = x.second;
	}

	return { std::move(f), s };
}

type Calculator::calculate(values&& args, std::vector<type>&& templates) const
{
	return result->calculate(std::move(args), std::move(templates));
}