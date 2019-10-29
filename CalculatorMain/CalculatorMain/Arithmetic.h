#pragma once
#include "Helper.h"
#include <sstream>

struct arthm : interface {
	const std::string& get_prompt() const override {
		static const std::string PROMPT {">>>"};
		return PROMPT;
	}
	double calc(std::istringstream& is) const;
};