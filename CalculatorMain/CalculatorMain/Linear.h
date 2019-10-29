#pragma once
#include "Helper.h"
#include <string>
#include <sstream>

struct linear : interface {
	const std::string& get_prompt() const override {
		static const std::string PROMPT {"L>>>"};
		return PROMPT;
	}
	double calc(std::istringstream& is) const;
};