#pragma once
#include <iostream>
#include <format>

namespace fmc {
    struct Money {
        int dollars = 0;
        int cents = 0;

        Money(int dollars_, int cents_) : dollars{dollars_}, cents{cents_} {}
        Money(double v) {
            this->dollars = (int) v;
            this->cents = (int) ((v - ((double) (int) v)) * 100);
            if (v < 0) {
                this->dollars -= 1;
                this->cents += 100;
            }
        }

        double to_double() const {
            return (double) this->dollars + ((double) this->cents) / 100;
        }

        void add(const Money& m) {
            this->dollars += m.dollars;
            this->cents += m.cents;

            this->dollars += this->cents / 100;
            this->cents %= 100;
        }

        Money operator+(const Money& m) const {
            int sum_dollars = m.dollars + this->dollars;
            int sum_cents = m.cents + this->cents;

            sum_dollars += sum_cents / 100;
            sum_cents %= 100;

            return Money{sum_dollars, sum_cents};
        }
        Money operator+=(const Money& m) {
            this->dollars += m.dollars;
            this->cents += m.cents;

            this->dollars += this->cents / 100;
            this->cents %= 100;

            return *this;
        }

        Money operator-(const Money& m) const {
            int diff_dollars = this->dollars - m.dollars;
            int diff_cents = this->cents - m.cents;

            if (diff_cents < 0) {
                diff_cents += 100;
                diff_dollars -= 1;
            }

            return Money{diff_dollars, diff_cents};
        }
        Money operator-=(const Money& m) {
            this->dollars -= m.dollars;
            this->cents -= m.cents;

            if (this->cents < 0) {
                this->dollars -= 1;
                this->cents += 100;
            }

            return *this;
        }

        Money operator*(double d) const {
            // This probably isn't great because this'll lose precision but for this sim I don't care much.
            return this->to_double() * d;
        }

        Money operator*=(double d) const {
            return *this * d;
        }

        // This is so trucking cool.
        auto operator<=>(const Money& other) const = default;
    };
}

namespace std {
    template <>
    struct formatter<fmc::Money> {
        constexpr auto parse(format_parse_context& ctx) {
            return ctx.begin(); // no custom format specifiers
        }

        template <typename FormatContext>
        auto format(const fmc::Money& m, FormatContext& ctx) const {
            return std::format_to(ctx.out(), "{}.{:02}", m.dollars, m.cents);
        }
    };
} // namespace std