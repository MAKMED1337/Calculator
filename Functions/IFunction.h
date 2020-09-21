#pragma once
#include <optional>
#include <vector>
#include <memory>
#include <unordered_map>

class IFunction;

using type = long double;
using values = std::unordered_map<std::string, std::unique_ptr<IFunction>>;
using child = std::vector<std::unique_ptr<IFunction>>;

class IFunction
{
public:
	virtual void build(child&& args) = 0;
	virtual type calculate(values const& args) const = 0;

	virtual std::unique_ptr<IFunction> create() const = 0;
	virtual std::unique_ptr<IFunction> create(child&& args) const = 0;

	virtual ~IFunction() = default;
};

#define factory(name)\
virtual std::unique_ptr<IFunction> create() const\
{\
	return std::make_unique<name>();\
}\
\
virtual std::unique_ptr<IFunction> create(child&& args) const\
{\
	auto t = create();\
	t->build(std::move(args));\
\
	return t;\
}