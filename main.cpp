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
	std::shared_ptr<FunctionNamespace> d = std::make_shared<FunctionNamespace>();

	Calculator t("x ^ 2 + 3", ns, d, bin_op);
	d->AddFunction("t", t.get_result(), { {"x"} });

	Calculator f("3 + t(y)", ns, d, bin_op);

	values vals;

	vals["y"] = 3;

	std::cout << f.calculate(std::move(vals)) << "\n\n";

	Calculator a("1", ns, d, bin_op);
	d->AddFunction("X", a.get_result(), { {} });

	Calculator b("x * X(...)", ns, d, bin_op);
	d->AddFunction("X", b.get_result(), { {"x", "..."} });

	std::cout << d->call("X", {1, 2, 3, 4, 5}) << "\n\n";

	Calculator c("f (3)", ns, d, bin_op);
	std::cout << c.calculate() << "\n\n";
}

cfunc_ptr get_func(std::vector<std::string> const& names, func_ptr&& f)
{
	size_t n = names.size();
	child a(n);
	for (size_t i = 0; i < n; ++i)
		a[i] = std::make_shared<const Variable>(names[i]);

	f->build(std::move(a));
	return f;
}

int main()
{
	std::cout << std::setprecision(999);

	ns->AddFunction("plus",
		get_func({ "x", "y" }, std::make_shared<Plus>()),
		{ {"x", "y"} });

	ns->AddFunction("minus",
		get_func({ "x", "y" }, std::make_shared<Minus>()),
		{ {"x", "y"} });

	ns->AddFunction("mult",
		get_func({ "x", "y" }, std::make_shared<Multiply>()),
		{ {"x", "y"} });

	ns->AddFunction("pow",
		get_func({ "x", "y" }, std::make_shared<Power>()),
		{ {"x", "y"} });

	ns->AddFunction("root",
		get_func({ "x", "y" }, std::make_shared<Root>()),
		{ {"x", "y"} });

	ns->AddFunction("log",
		get_func({ "x", "y" }, std::make_shared<Logarithm>()),
		{ {"x", "y"} });

	ns->AddFunction("div",
		get_func({ "x", "y" }, std::make_shared<Division>()),
		{ {"x", "y"} });
	

	bin_op['+'] = { 0, 0, "plus" };
	bin_op['-'] = { 0, 1, "minus" };

	bin_op['*'] = { 1, 1, "mult" };
	bin_op['/'] = { 1, 2, "div" };

	bin_op['^'] = { 1000, 1000, "pow" };


	test();

	std::string cmd;
	std::getline(std::cin, cmd);

	Calculator f(cmd, {}, ns, bin_op);

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