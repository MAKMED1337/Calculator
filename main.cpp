#include "Functions/IFunction.h"
#include "Functions/Plus.cpp"
#include "Functions/Minus.cpp"
#include "Functions/Multiply.cpp"
#include "Functions/Power.cpp"
#include "Functions/Root.cpp"
#include "Functions/Logarithm.cpp"
#include "Functions/Division.cpp"

#include "Calculator.h"

#include <iostream>
#include <iomanip>

std::shared_ptr<FunctionNamespace> ns = std::make_shared<FunctionNamespace>();

std::array<Calculator::binary_operator, alphabet_length> bin_op;

#include <chrono>
using namespace std::chrono;

long long get_ms()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
		).count();
}

void test()
{
	Calculator t("x ^ 2 + 3", ns, bin_op);
	ns->AddFunction("t", t.get_func(), { {"x"} });

	Calculator f("3 + t(y)", ns, bin_op);

	values vals;

	vals["y"] = 3;

	std::cout << f.calculate(std::move(vals)) << "\n\n";

	Calculator a("1", ns, bin_op);
	ns->AddFunction("X", a.get_func(), { {} });

	Calculator b("x * X(...)", ns, bin_op);
	ns->AddFunction("X", b.get_func(), { {"x", "..."} });

	std::cout << ns->call("X", {1, 2, 3, 4, 5}) << "\n\n";
}

func_ptr get_func(std::vector<std::string> const& names, func_ptr&& f)
{
	uint64_t n = names.size();
	std::vector<func_ptr> a(n);
	for (uint64_t i = 0; i < n; ++i)
		a[i] = make<Variable>(names[i]);

	f->build(std::move(a));
	return f;
}

int main()
{
	std::cout << std::setprecision(999);

	bin_op['+'] = { 0, 0, std::make_shared<Plus>() };
	bin_op['-'] = { 0, 1, std::make_shared<Minus>() };

	bin_op['*'] = { 1, 1, std::make_shared<Multiply>() };
	bin_op['/'] = { 1, 2, std::make_shared<Division>() };

	bin_op['^'] = { 1000, 1000, std::make_shared<Power>() };


	ns->AddFunction("plus",
		get_func({"x", "y"}, make<Plus>()),
		{ {"x", "y"} });

	ns->AddFunction("minus",
		get_func({ "x", "y" }, make<Minus>()),
		{ {"x", "y"} });

	ns->AddFunction("mult",
		get_func({ "x", "y" }, make<Multiply>()),
		{ {"x", "y"} });

	ns->AddFunction("power",
		get_func({ "x", "y" }, make<Power>()),
		{ {"x", "y"} });

	ns->AddFunction("root",
		get_func({ "x", "y" }, make<Root>()),
		{ {"x", "y"} });

	ns->AddFunction("log",
		get_func({ "x", "y" }, make<Logarithm>()),
		{ {"x", "y"} });

	ns->AddFunction("div",
		get_func({ "x", "y" }, make<Division>()),
		{ {"x", "y"} });
	
	test();

	std::string cmd;
	std::getline(std::cin, cmd);

	Calculator f(cmd, ns, bin_op);

	values vals;

	do
	{
		std::string name;
		std::cin >> name;

		if (name == "exit")
			break;

		type t;
		std::cin >> t;

		vals[name] = { t };
	} while (true);

	std::cout << std::setprecision(999) << f.calculate(std::move(vals)) << "\n";
#ifndef _DEBUG
	system("pause");
#endif


	/*std::cerr << "test:\n";
	auto beg = get_ms();
	for (int i = 0; i < 1e6; ++i)
		Calculator(cmd, func, bin_op);
	auto end = get_ms();
	std::cerr << end - beg << " ms\n";

	beg = get_ms();
	for (int i = 0; i < 1e6; ++i)
		f.calculate(vals);
	end = get_ms();

	std::cerr << end - beg << " ms\n";*/
}