#pragma once
#include <ostream>

class rational {
public:
	rational() = delete;
	rational(int _nu, int _de);
	int numerator() const noexcept { return nu; }
	int denominator() const noexcept { return de; }

	int& numerator() noexcept { return nu; }
	int& denominator() noexcept { return de; }

	friend std::ostream& operator<<(std::ostream& os, rational r);
	friend rational operator+(rational a, rational b);

	friend rational operator+(int a, rational b) {
		return {b.nu + a * b.de, b.de};
	}
	friend rational operator+(rational a, int b) {
		return b + a;
	}
	friend bool operator==(rational a, rational b) {
		return a.nu / a.de == b.nu / b.de;
	}
	

private:
	int nu;
	int de;

	rational simplify();
};