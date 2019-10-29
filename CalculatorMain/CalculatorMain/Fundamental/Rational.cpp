#include "Rational.h"
#include <limits>
#include <stdexcept>
#include <numeric>

using namespace std;

rational::rational(int _nu, int _de)
	: nu {_nu}
{
	if (_de == 0) {
		throw invalid_argument("denominator value 0");
	}
	de = _de;
}

rational rational::simplify() {
	int _gcd = 0;
	while (_gcd = gcd(nu, de) != 1) {
		de /= _gcd;
		nu /= _gcd;
	}

	return *this;
}

rational operator+(rational a, rational b) {
	int _lcm = lcm(a.de, b.de);
	rational temp {a.nu * _lcm / a.de + b.nu * _lcm / b.de, _lcm};
	return temp.simplify();
}

ostream& operator<<(ostream& os, rational r) {
	return os << r.nu << '/' << r.de;
}