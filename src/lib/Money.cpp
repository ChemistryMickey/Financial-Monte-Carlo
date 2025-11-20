#include "Money.hpp"

namespace fmc {

    Money::Money(int dollars_, int cents_) : dollars{dollars_}, cents{cents_} {}
    Money::Money(double v) {
        // Doing this without loss is actually kinda rough.
        this->dollars = (int) v;
        this->cents = (int) ((v - ((double) (int) v)) * 100);
        if (v < 0) {
            this->dollars -= 1;
            this->cents += 100;
        }
    }

    double Money::to_double() const {
        return (double) this->dollars + ((double) this->cents) / 100;
    }

    Money Money::operator+(const Money& m) const {
        int sum_dollars = m.dollars + this->dollars;
        int sum_cents = m.cents + this->cents;

        sum_dollars += sum_cents / 100;
        sum_cents %= 100;

        return Money{sum_dollars, sum_cents};
    }
    Money& Money::operator+=(const Money& m) {
        *this = *this + m;

        return *this;
    }

    Money Money::operator-(const Money& m) const {
        int diff_dollars = this->dollars - m.dollars;
        int diff_cents = this->cents - m.cents;

        if (diff_cents < 0) {
            diff_cents += 100;
            diff_dollars -= 1;
        }

        return Money{diff_dollars, diff_cents};
    }
    Money& Money::operator-=(const Money& m) {
        *this = *this - m;

        return *this;
    }

    Money Money::operator*(int i) const {
        int new_dol = this->dollars * i;
        int new_cents = this->cents * i;

        new_dol += new_cents / 100;
        new_cents %= 100;

        return {new_dol, new_cents};
    }

    Money Money::operator*(double d) const {
        return this->to_double() * d;
    }

    Money& Money::operator*=(double d) {
        *this = *this * d;
        return *this;
    }
}