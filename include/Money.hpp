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
            this->cents = (int) (v - (double) (int) v);
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
            return std::format_to(ctx.out(), "${}.{}", m.dollars, m.cents);
        }
    };
} // namespace std