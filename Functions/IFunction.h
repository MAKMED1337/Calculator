#pragma once
//#include "FunctionArguments.h"

#include <optional>
#include <vector>
#include <memory>
#include <unordered_map>

class IFunction;

using type = long double;
using func_ptr = std::shared_ptr<IFunction>;

struct FunctionArgumentsData
{
	std::vector<std::string> names;
	bool is_template = false;
};

class FunctionArguments final :
	public std::vector<func_ptr>
{
public:
	FunctionArguments() {}

	FunctionArguments(func_ptr f);

	void operator= (func_ptr f);

	void unite(func_ptr f);

	void unite(FunctionArguments const& fa);

	std::unordered_map<std::string, FunctionArguments>
		superimpose(FunctionArgumentsData const& fad) const;
};

using values = std::unordered_map<std::string,
	FunctionArguments>;

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
	virtual void build(child&& args) = 0;
	virtual type calculate(values const& args) const = 0;

	virtual func_ptr create() const = 0;
	virtual func_ptr create(child&& args) const = 0;

	virtual ~IFunction() = default;
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