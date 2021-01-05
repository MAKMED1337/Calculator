#pragma once
#include <optional>
#include <vector>
#include <memory>
#include <unordered_map>

class IFunction;

using type = long double;

//null means template
using func_ptr = std::shared_ptr<IFunction>;

using values = std::unordered_map<std::string, type>;

using child = std::vector<func_ptr>;

template <class T>
std::shared_ptr<T> make()
{
	return std::make_shared<T>();
}

template <class T, class ...Y>
std::shared_ptr<T> make(Y const& ...t)
{
	return std::make_shared<T>(t...);
}

class IFunction
{
public:
	virtual void build(child&& args)
	{
		todo = std::move(args);
	}

	virtual type calculate(values const& args, std::vector<type> const& templates) const
	{
		return calculate(std::move(flat(args, templates)));
	}

	virtual func_ptr create() const = 0;
	virtual func_ptr create(child&& args) const = 0;

	virtual ~IFunction() = default;
protected:
	child todo;
	virtual std::vector<type> flat(values const& args, std::vector<type> const& templates) const
	{
		std::vector<type> res;
		for (auto const& i : todo)
			if (i)
				res.push_back(i->calculate(args, templates));
			else
				res.insert(res.end(), templates.cbegin(), templates.cend());

		return res;
	}

	virtual type calculate(std::vector<type>&& args) const = 0;
};

#define factory(name)\
virtual func_ptr create() const\
{\
	return make<name>();\
}\
\
virtual func_ptr create(child&& args) const\
{\
	auto t = create();\
	t->build(std::move(args));\
\
	return t;\
}