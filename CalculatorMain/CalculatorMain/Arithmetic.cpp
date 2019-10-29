#include "Arithmetic.h"

using namespace std;

static void clean_up(const string& temp) {
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

static void parse(istringstream& is) {
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
		else if (next == '(') {
			if (!checkTType(lastTType, token_t::oparen)) {
				throw invalid_argument("unexpected opening parenthesis");
			}
			if (lastTType == token_t::num || lastTType == token_t::var || lastTType == token_t::cparen) {
				clean_up("*");
				operators.push_back("*");
			}
			braceLim.push_back(operators.size());
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
		}
		else {
			is.unget();
			string temp {name(is)};
			if (funcTable.find(temp) != end(funcTable)) {
				if (!checkTType(lastTType, token_t::func)) {
					throw invalid_argument("unexpected function call");
				}
				if (lastTType == token_t::num || lastTType == token_t::var || lastTType == token_t::cparen) {
					clean_up("*");
					operators.push_back("*");
				}
				operators.push_back(temp);
				lastTType = token_t::func;
			}
			else if (varTable.find(temp) != end(varTable)) {
				if (!checkTType(lastTType, token_t::var)) {
					throw invalid_argument("unexpected variable");
				}
				if (lastTType == token_t::num || lastTType == token_t::var || lastTType == token_t::cparen) {
					clean_up("*");
					operators.push_back("*");
				}
				operands.push_back(varTable[temp]);
				lastTType = token_t::var;
			}
			else if (opTable.find(temp) != end(opTable)) {
				if (!checkTType(lastTType, token_t::opr)) {
					throw invalid_argument("unexpected operator");
				}
				clean_up(temp);
				operators.push_back(temp);
				lastTType = token_t::opr;
			}
			else {
				throw invalid_argument("unrecognized token");
			}
		}
	}
	if (!checkTType(lastTType, token_t::none)) {
		throw invalid_argument("invalid expression");
	}
}

double arthm::calc(istringstream& is) const {
	operands.clear();
	parse(is);
	while (!operators.empty()) {
		operate(operators.back());
		operators.pop_back();
	}
	return operands[0];
}
