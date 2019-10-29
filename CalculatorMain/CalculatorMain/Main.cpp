#include "Helper.h"
#include "Arithmetic.h"
#include "Linear.h"
#include "Fundamental/Rational.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <regex>
#include <unordered_map>
#include <iomanip>
#include <nlohmann/json.hpp>

#define VER (0.5)

using namespace std;
using json = nlohmann::json;

enum class mode_t {
	arthm, linear
};

const interface& factory_func(mode_t mode) {
	static const arthm A;
	static const linear B;
	switch (mode)
	{
	case mode_t::arthm:
		return A;
	case mode_t::linear:
		return B;
	default:
		return A;
	}
}

namespace {
	json config;
	json configRange;
	mode_t mode;
}

void change_def_settings() {
	static const regex reg {R"((\d+)\s*,\s*(\d+)\s*)"};

	cout << left;
	std::for_each(config.items().begin(), config.items().end(), [i = 1] (auto& val) mutable {
		cout << i++ << ": " << setw(16) << val.key() << " -> " << val.value() << "\n";
	});

	string in;
	smatch match;
	getline(cin, in);
	if (regex_match(in, match, reg)) {
		int choice = stoi(match[1]);
		int val = stoi(match[2]);
		if (choice > config.size()) {
			cerr << "invalid selection\n";
		}
		json::iterator it = config.begin();
		advance(it, choice - 1);
		if (val < configRange[it.key()][0] || val > configRange[it.key()][1]) {
			cerr << "invalid value\n";
			return;
		}
		it.value() = stoi(match[2]);
	}
	else {
		cerr << "to change settings, use the format: #,#";
	}

	cout << right;
}

void save_s() {
	ofstream of {"config.json"};
	of << setw(4) << config;
}

const unordered_map<string, function<void(void)>> comTable {
	{"exit", [] {
		exit(0);
	}},
	{"settings", change_def_settings},
	{"linear", [] {
		mode = mode_t::linear;
		cout << '\n';
	}},
	{"arithmetic", [] {
		mode = mode_t::arthm;
		cout << '\n';
	}}
};

int main() {
	cout << "************************************\n";
	cout << "** ADVANCED CALCULATOR BY TONY HU\n";
	cout << "** VERSION: " << VER << '\n';
	cout << "************************************\n\n";

	atexit(save_s);

	ifstream is {"config.json"};
	ifstream is2 {"configRange.json"};
	is >> config;
	is2 >> configRange;

	varTable["_lastAns"] = 0.0;
	mode = static_cast<mode_t>(config["defaultMode"]);
	
	while (true) {
		string in;
		cout << factory_func(mode).get_prompt();
		getline(cin, in);
		// multi-line statements
		size_t ind = in.find_last_not_of(' ');
		size_t ind2 = in.find_first_not_of(' ');
		if (ind == string::npos)
			continue;

		try {
			comTable.at(in.substr(ind2, ind + 1))();
			continue;
		}
		catch (out_of_range) {}

		while (in[ind] == '\\') {
			string nextStr;
			cout << "  ";
			getline(cin, nextStr);
			in = in.substr(0, ind) + nextStr;
			ind = in.find_last_not_of(' ');
		}
		if (mode == mode_t::arthm) {
			//variable assignment
			/*
			 * a valid variable name consists of a letter or _ followed
			 * by any number of word characters
			 */
			regex assignment {R"(([[:alpha:]_]\w*)\s*=\s*(\d*(\.\d+)?)\s*)"};
			smatch data;
			if (regex_match(in, data, assignment)) {
				if (funcTable.find(data[1]) != end(funcTable)) {
					cerr << "variable names already taken by functions\n";
				}
				else if (comTable.find(data[1]) != end(comTable)) {
					cerr << "variable names already taken by commands\n";
				}
				else {
					varTable[data[1]] = stod(data[2]);
				}
				continue;
			}
		}
		istringstream is {in};
		try {
			varTable["_lastAns"] = factory_func(mode).calc(is);
			cout << varTable["_lastAns"] << "\n";
		}
		catch (invalid_argument& ia) {
			cerr << ia.what() << "\n";
		}
		clear_all();
	}
}
