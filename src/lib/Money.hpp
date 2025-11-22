#pragma once
#include <iostream>
#include <format>

namespace fmc {
    struct Money {
        int dollars = 0;
        int cents = 0;

        Money(int dollars_, int cents_);
        Money(double v);

        double to_double() const;

        Money operator+(const Money& m) const;
        Money& operator+=(const Money& m);

        Money operator-(const Money& m) const;
        Money& operator-=(const Money& m);

        Money operator*(uint i) const;
        Money operator*(int i) const;
        Money operator*(double d) const;
        Money& operator*=(double d);

        // This is so trucking cool.
        auto operator<=>(const Money& other) const = default;

    private:
        void normalize();
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
            return std::format_to(ctx.out(), "{}.{:02}", m.dollars, std::abs(m.cents));
        }
    };
} // namespace std