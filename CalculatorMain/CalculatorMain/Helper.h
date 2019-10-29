#pragma once
#include <sstream>
#include <unordered_map>
#include <functional>

const std::unordered_map<std::string, std::function<double(double)>> funcTable {
	{"sqrt", [](double d) {
		return std::sqrt(d);
	}},
	{"log", [](double d) {
		return std::log(d);
	}},
};
const std::unordered_map<std::string, int> opTable {
	{"+", 0},
	{"-", 0},
	{"*", 1},
	{"/", 1},
	{"^", 2}
};

extern std::unordered_map<std::string, double> varTable;
extern std::vector<double> operands;
extern std::vector<unsigned int> braceLim;
extern std::list<std::string> operators;
constexpr int FUNC_P = 3;

struct interface
{
	virtual const std::string& get_prompt() const = 0;
	virtual double calc(std::istringstream&) const = 0;
	virtual ~interface() {}
};



enum class token_t {
	none, num, opr, oparen, cparen, func, var
};

std::string name(std::istringstream&);
double num(std::istringstream&);
bool checkTType(token_t, token_t);
void operate(const std::string&);
void clear_all();