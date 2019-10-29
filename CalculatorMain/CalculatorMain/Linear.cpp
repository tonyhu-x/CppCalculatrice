#include "Linear.h"
#include <regex>
#include <limits>
#include <utility>
#include <algorithm>

using namespace std;

using dlimits = numeric_limits<double>;

string varName {"x"};

static void clean_up(const string& temp) {
	replace(begin(operands), end(operands), dlimits::infinity(), 1.0);
	while (operators.size() > (braceLim.empty() ? 0 : braceLim.back())) {
		int lastP;
		if (opTable.find(operators.back()) == end(opTable))
			lastP = FUNC_P;
		else
			lastP = opTable.at(operators.back());
		if (opTable.at(temp) < lastP) {
			operate(operators.back());
			operators.pop_back();
		}
		else break;
	}
}

static auto parse(istringstream& is) {
	double coefficient = 0;
	double constant = 0;
	bool isCoe = false;
	bool isPositive = true;
	token_t lastTType {token_t::none};

	for (char next; is >> next;) {
		if ((isdigit(next))) {
			if (!checkTType(lastTType, token_t::num)) {
				throw invalid_argument("unexpected number token");
			}
			is.unget();
			operands.push_back(num(is));
			lastTType = token_t::num;
		}
		else if (next == '+' && !checkTType(lastTType, token_t::opr)) {
			lastTType = token_t::opr;
		}
		else if (next == '-' && !checkTType(lastTType, token_t::opr)) {
			operands.push_back(-1);
			clean_up("*");
			operators.push_back("*");
			lastTType = token_t::opr;
		}
		/*else if (next == '(') {
			if (!checkTType(lastTType, token_t::oparen)) {
				throw invalid_argument("unexpected opening parenthesis");
			}
			if ()
			lastTType = token_t::oparen;
		}
		else if (next == ')') {
			if (!checkTType(lastTType, token_t::cparen))
				throw invalid_argument("unexpected closing parenthesis");
			if (braceLim.empty())
				throw invalid_argument("no matching opening parenthesis");
			while (operators.size() > braceLim.back()) {
				operate(operators.back());
				operators.pop_back();
			}
			braceLim.pop_back();
			lastTType = token_t::cparen;
		}*/
		else {
			is.unget();
			string temp {name(is)};
			if (temp == varName) {
				if (!operators.empty()) {
					const string& last = operators.back();
					if (isCoe
						|| last == "/"
						|| last == "^"
						|| funcTable.find(last) != end(funcTable)) {
						throw invalid_argument("not a linear function");
					}
				}
				isCoe = true;
				operands.push_back(dlimits::infinity());
				lastTType = token_t::var;
			}
			else if (funcTable.find(temp) != end(funcTable)) {
				if (!checkTType(lastTType, token_t::func)) {
					throw invalid_argument("unexpected function call");
				}
				operators.push_back(temp);
				lastTType = token_t::func;
			}
			else if (varTable.find(temp) != end(varTable)) {
				if (!checkTType(lastTType, token_t::var)) {
					throw invalid_argument("unexpected variable");
				}
				operands.push_back(varTable[temp]);
				lastTType = token_t::var;
			}
			else if (opTable.find(temp) != end(opTable)) {
				if (!checkTType(lastTType, token_t::opr)) {
					throw invalid_argument("unexpected operator");
				}
				if (operands.back() == numeric_limits<double>::infinity() && temp == "^") {
					throw invalid_argument("not a linear function");
				}
				if (temp == "+" || temp == "-") {
					clean_up(temp);
					if (isCoe) {
						if (isPositive) {
							coefficient += operands[0];
						}
						else {
							coefficient -= operands[0];
						}
					}
					else {
						if (isPositive) {
							constant += operands[0];
						}
						else {
							constant -= operands[0];
						}
					}
					isPositive = temp == "+" ? true : false;
					isCoe = false;
					clear_all();
				}
				else {
					clean_up(temp);
					operators.push_back(temp);
				}
				lastTType = token_t::opr;
			}
			else {
				throw invalid_argument("unrecognized token");
			}
		}
	}
	replace(begin(operands), end(operands), dlimits::infinity(), 1.0);
	while (!operators.empty()) {
		operate(operators.back());
		operators.pop_back();
	}
	if (isCoe) {
		if (isPositive) {
			coefficient += operands[0];
		}
		else {
			coefficient -= operands[0];
		}
	}
	else {
		if (isPositive) {
			constant += operands[0];
		}
		else {
			constant -= operands[0];
		}
	}
	return make_pair(coefficient, constant);
}

double linear::calc(istringstream& is) const {
	const string str {is.str()};
	regex test {R"(([^=]+)=([^=]+))"};
	smatch match;
	if (!regex_match(str, match, test)) {
		throw invalid_argument("ill-formed equation");
	}
	istringstream s1 {match[1]};
	istringstream s2 {match[2]};
	auto p1 = parse(s1);
	clear_all();
	auto p2 = parse(s2);
	if (p1.second == p2.second)
		return 0;
	else if (p1.first == p2.first)
		throw invalid_argument("any num");
	else
		return (p1.second - p2.second) / (p2.first - p1.first);
}