#include "Helper.h"

using namespace std;

unordered_map<string, double> varTable;
vector<double> operands;
vector<unsigned int> braceLim;
list<string> operators;

void clear_all() {
	operands.clear();
	braceLim.clear();
	operators.clear();
}

double num(istringstream& is) {
	double result = 0;
	if (!(is >> result)) {
		throw invalid_argument("error: expected a number");
	}
	return result;
}

/**
 * Reads a variable name.
 */
string name(istringstream& is) {
	string temp;
	char next;
	is >> next;
	if (!(isalpha(next) || next == '_'))
		return {next};
	temp += next;
	for (; is.get(next) && (isalnum(next) || next == '_');)
		temp += next;
	is.unget();
	return temp;
}

bool checkTType(token_t last, token_t cur) {
	switch (last) {
	case token_t::none:
	case token_t::opr:
	case token_t::oparen:
	case token_t::func:
		return cur != token_t::cparen && cur != token_t::opr && cur != token_t::none;
	case token_t::num:
		return cur != token_t::num;
	case token_t::cparen:
	case token_t::var:
		return cur != token_t::num && cur != token_t::var;
	default:
		throw invalid_argument("???");
	}
}

void operate(const string& which) {
	if (which == "+") {
		operands[operands.size() - 2] += operands.back();
		operands.pop_back();
	}
	else if (which == "-") {
		operands[operands.size() - 2] -= operands.back();
		operands.pop_back();
	}
	else if (which == "*") {
		operands[operands.size() - 2] *= operands.back();
		operands.pop_back();
	}
	else if (which == "/") {
		operands[operands.size() - 2] /= operands.back();
		operands.pop_back();
	}
	else if (which == "^") {
		operands[operands.size() - 2] = pow(operands[operands.size() - 2], operands.back());
		operands.pop_back();
	}
	else {
		operands.back() = funcTable.at(which)(operands.back());
		if (isnan(operands.back()))
			throw invalid_argument("math error");
	}
}