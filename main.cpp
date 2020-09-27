#include "Functions/IFunction.h"
#include "Functions/Plus.cpp"
#include "Functions/Minus.cpp"
#include "Functions/Multiply.cpp"
#include "Functions/Power.cpp"
#include "Functions/Root.cpp"
#include "Functions/Logarithm.cpp"
#include "Functions/Division.cpp"

#include "FunctionArguments.h"

#include "Calculator.h"

#include <iostream>
#include <iomanip>

std::unordered_map<std::string_view, std::shared_ptr<IFunction>> func;

std::array<Calculator::binary_operator, alphabet_length> bin_op;

#include <chrono>
using namespace std::chrono;

long long get_ms()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
		).count();
}

int main()
{
	bin_op['+'] = { 0, 0, std::make_shared<Plus>() };
	bin_op['-'] = { 0, 1, std::make_shared<Minus>() };

	bin_op['*'] = { 1, 1, std::make_shared<Multiply>() };
	bin_op['/'] = { 1, 2, std::make_shared<Division>() };

	bin_op['^'] = { 1000, 1000, std::make_shared<Power>() };


	func["plus"] = std::make_shared<Plus>();
	func["minus"] = std::make_shared<Minus>();
	func["mult"] = std::make_shared<Multiply>();
	func["power"] = std::make_shared<Power>();
	func["root"] = std::make_shared<Root>();
	func["log"] = std::make_shared<Logarithm>();
	func["div"] = std::make_shared<Division>();
	
	std::string cmd;
	std::getline(std::cin, cmd);

	Calculator f(cmd, func, bin_op);

	values vals;

	do
	{
		std::string name;
		std::cin >> name;

		if (name == "exit")
			break;

		type t;
		std::cin >> t;

		vals[name] = get_const(t);
	} while (true);

	std::cout << std::setprecision(999) << f.calculate(vals) << "\n";

	std::cerr << "test:\n";
	auto beg = get_ms();
	for (int i = 0; i < 1e6; ++i)
		Calculator(cmd, func, bin_op);
	auto end = get_ms();
	std::cerr << end - beg << " ms\n";

	beg = get_ms();
	for (int i = 0; i < 1e6; ++i)
		f.calculate(vals);
	end = get_ms();

	std::cerr << end - beg << " ms\n";
}