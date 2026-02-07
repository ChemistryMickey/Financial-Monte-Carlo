#include "gtest/gtest.h"
#include "Money.hpp"
#include <cinttypes>

namespace fmc {
    TEST(Test_Money, init) {
        { // Positive
            Money m{42.808};
            EXPECT_EQ(m.dollars, 42);
            EXPECT_EQ(m.cents, 81);
        }
        { // Negative
            Money m{-42.808};
            EXPECT_EQ(m.dollars, -42);
            EXPECT_EQ(m.cents, -81);
        }
    }

    TEST(Test_Money, addition) {
        {
            Money m1{42};
            Money m2{808};

            Money s = m1 + m2;

            EXPECT_EQ(s.dollars, 850);
            EXPECT_EQ(s.cents, 0);
        }
        {
            Money m1{0.2};
            EXPECT_EQ(m1.cents, 20);
            Money m2{0.541247788}; // You may get horrible doubles.
            EXPECT_EQ(m2.cents, 54);

            Money s = m1 + m2;

            EXPECT_EQ(s.dollars, 0);
            EXPECT_EQ(s.cents, 74);
        }
        {
            Money m1{0.2};
            EXPECT_EQ(m1.cents, 20);
            Money m2{0.541247788};
            EXPECT_EQ(m2.cents, 54);

            m1 += m2;

            EXPECT_EQ(m1.dollars, 0);
            EXPECT_EQ(m1.cents, 74);
        }
        {
            Money m1{0.8};
            EXPECT_EQ(m1.cents, 80);
            Money m2{0.541247788};
            EXPECT_EQ(m2.cents, 54);

            Money s = m1 + m2;

            EXPECT_EQ(s.dollars, 1);
            EXPECT_EQ(s.cents, 34);
        }
    }
    TEST(Test_money, negative_addition) {
        Money m1{-0.8};
        EXPECT_EQ(m1.dollars, 0);
        EXPECT_EQ(m1.cents, -80);
        Money m2{-0.541247788};
        EXPECT_EQ(m2.dollars, 0);
        EXPECT_EQ(m2.cents, -54);

        Money s = m1 + m2;

        EXPECT_EQ(s.dollars, -1);
        EXPECT_EQ(s.cents, -34);
    }

    TEST(Test_Money, subtraction) {
        {
            Money m1{42};
            Money m2{3};

            Money d = m1 - m2;
            EXPECT_EQ(d.dollars, 39);
            EXPECT_EQ(d.cents, 0);
        }
        {
            Money m1{42};
            Money m2{3.5};

            Money d = m1 - m2;
            EXPECT_EQ(d.dollars, 38);
            EXPECT_EQ(d.cents, 50);
        }
        { // it is allowed to be negative. Plenty of people have negative net worth.
            Money m1{42};
            Money m2{43};

            Money d = m1 - m2;
            EXPECT_EQ(d.dollars, -1);
            EXPECT_EQ(d.cents, 0);
        }
    }

    TEST(Test_Money, negative_subtraction) {
        Money m1{42};
        Money m2{42.5};

        Money d = m1 - m2;
        EXPECT_EQ(d.dollars, 0);
        EXPECT_EQ(d.cents, -50);
    }

    TEST(Test_Money, multiplication) {
        {
            Money m{42};
            Money m2 = m * (int64_t) 2;

            EXPECT_EQ(m2.dollars, 84);
            EXPECT_EQ(m2.cents, 0);
        }
        {
            Money m{42.5};
            Money m2 = m * (int64_t) 2;

            EXPECT_EQ(m2.dollars, 85);
            EXPECT_EQ(m2.cents, 0);
        }
        {
            Money m{42, 30};
            EXPECT_EQ(m.cents, 30);
            Money m2 = m * (int64_t) 2;

            EXPECT_EQ(m2.dollars, 84);
            EXPECT_EQ(m2.cents, 60);
        }
        {
            Money m{40, 80};
            EXPECT_EQ(m.cents, 80);
            Money m2 = m * 0.25;

            EXPECT_EQ(m2.dollars, 10);
            EXPECT_EQ(m2.cents, 20);
        }
        {
            Money m{40, 80};
            EXPECT_EQ(m.cents, 80);
            Money m2 = m * 0.025;

            EXPECT_EQ(m2.dollars, 1);
            EXPECT_EQ(m2.cents, 2);
        }
    }

    TEST(Test_Money, equal_do_operators_plus) {
        {
            Money m{42, 0};
            m += 2;
            EXPECT_EQ(m.dollars, 44);
        }
        {
            Money m{42, 47};
            m += 2.5;
            EXPECT_EQ(m.dollars, 44);
            EXPECT_EQ(m.cents, 97);
        }
    }
    TEST(Test_Money, equal_do_operators_minus) {
        {
            Money m{42, 0};
            m -= 2;
            EXPECT_EQ(m.dollars, 40);
        }
        {
            Money m{42, 0};
            m -= 2.5;
            EXPECT_EQ(m.dollars, 39);
            EXPECT_EQ(m.cents, 50);
        }
    }
    TEST(Test_Money, equal_do_operators_multiply) {
        {
            Money m{42, 0};
            m *= 2;
            EXPECT_EQ(m.dollars, 84);
        }
        {
            Money m{42, 0};
            m *= 2.5;
            EXPECT_EQ(m.dollars, 105);
            EXPECT_EQ(m.cents, 0);
        }
    }

    TEST(Test_Money, across_zero) {
        Money m1{5, 47};
        Money m2{10, 80};

        Money m3 = m1 - m2;
        EXPECT_TRUE(m3.cents < 0);
        EXPECT_TRUE(m3.dollars < 0);

        Money expected{-5, -33};
        EXPECT_EQ(m3.dollars, expected.dollars);
        EXPECT_EQ(m3.cents, expected.cents);
    }

    TEST(Test_Money, close) {
        Money m1{5, 47};
        Money m2{5, 49};

        Money m3 = m1 - m2;
        EXPECT_TRUE(m3.cents < 0);
        EXPECT_TRUE(m3.dollars == 0);

        Money expected{0, -2};
        EXPECT_EQ(m3.dollars, expected.dollars);
        EXPECT_EQ(m3.cents, expected.cents);
    }

    TEST(Test_Money, division) {
        {
            Money m1{10, 0};
            Money m2{2, 0};

            double div = m1 / m2;
            EXPECT_EQ(div, 5.0);
        }
        {
            Money m1{2, 0};
            Money m2{10, 0};

            double div = m1 / m2;
            EXPECT_EQ(div, 0.2);
        }
        {
            Money m1{2, 0};
            Money m2{10, 0};

            // Because this is how you resolve how many stock to dissolve
            int div = static_cast<int>(m1 / m2) + 1;
            EXPECT_EQ(div, 1);
        }
        {
            Money m1{10, 0};
            Money m2{2, 0};

            // Because this is how you resolve how many stock to dissolve
            int div = static_cast<int>(m1 / m2) + 1;
            EXPECT_EQ(div, 6);
        }
    }
}
