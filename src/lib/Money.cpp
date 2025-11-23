#include "Money.hpp"
#include <cmath>

namespace fmc {

    Money::Money(long long int dollars_, int cents_) : dollars{dollars_}, cents{cents_} {
        this->normalize();
    }
    Money::Money(double v) {
        long long int total = std::llround(v * 100);
        this->dollars = total / 100;
        this->cents = (int) total % 100;
        this->normalize();
    }

    void Money::normalize() {
        long long int total = this->dollars * 100 + this->cents;

        this->dollars = total / 100;
        this->cents = (int) total % 100;

        // Sign consistency
        if (this->dollars > 0 && this->cents < 0) {
            this->dollars -= 1;
            this->cents += 100;
        }
        if (this->dollars < 0 && this->cents > 0) {
            this->dollars += 1;
            this->cents -= 100;
        }
    }

    double Money::to_double() const {
        return (double) this->dollars + ((double) this->cents) / 100;
    }

    Money Money::operator+(const Money& m) const {
        Money result{this->dollars + m.dollars, this->cents + m.cents};
        result.normalize();

        return result;
    }
    Money& Money::operator+=(const Money& m) {
        *this = *this + m;

        return *this;
    }

    Money Money::operator-(const Money& m) const {
        Money result{this->dollars - m.dollars, this->cents - m.cents};
        result.normalize();

        return result;
    }
    Money& Money::operator-=(const Money& m) {
        *this = *this - m;

        return *this;
    }

    Money Money::operator*(uint i) const {
        long long int total = this->dollars * 100 + this->cents;
        total *= i;

        Money result{total / 100, (int) total % 100};
        result.normalize();

        return result;
    }
    Money Money::operator*(long long int i) const {
        long long int total = this->dollars * 100 + this->cents;
        total *= i;

        Money result{total / 100, (int) total % 100};
        result.normalize();

        return result;
    }

    Money Money::operator*(double d) const {
        return this->to_double() * d;
    }

    Money& Money::operator*=(double d) {
        *this = *this * d;
        return *this;
    }

    double Money::operator/(const Money& m) const {
        long long int cur_total = this->dollars * 100 + this->cents;
        long long int other_total = m.dollars * 100 + m.cents;

        return ((double) cur_total) / ((double) other_total);
    }
}