#include "gtest/gtest.h"
#include "Money.hpp"

namespace fmc {
    TEST(Test_Money, init) {
        { // Positive
            Money m{42.808};
            EXPECT_EQ(m.dollars, 42);
            EXPECT_EQ(m.cents, 80);
        }
        { // Negative
            Money m{-42.808};
            EXPECT_EQ(m.dollars, -43);
            EXPECT_EQ(m.cents, 20);
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
        EXPECT_EQ(m1.dollars, -1);
        EXPECT_EQ(m1.cents, 20);
        Money m2{-0.541247788};
        EXPECT_EQ(m2.dollars, -1);
        EXPECT_EQ(m2.cents, 46);

        Money s = m1 + m2;

        // This is kinda funky, but it means the dollars will be the only negative portion.
        // So the human-like answer is "$-1.34" but here, "$-2.00 + $0.66" is equivalent
        EXPECT_EQ(s.dollars, -2);
        EXPECT_EQ(s.cents, 66);
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
        EXPECT_EQ(d.dollars, -1);
        EXPECT_EQ(d.cents, 50);
    }

    TEST(Test_Money, multiplication) {
        {
            Money m{42};
            Money m2 = m * 2;

            EXPECT_EQ(m2.dollars, 84);
            EXPECT_EQ(m2.cents, 0);
        }
        {
            Money m{42.5};
            Money m2 = m * 2;

            EXPECT_EQ(m2.dollars, 85);
            EXPECT_EQ(m2.cents, 0);
        }
        {
            Money m{42, 30};
            EXPECT_EQ(m.cents, 30);
            Money m2 = m * 2;

            EXPECT_EQ(m2.dollars, 84);
            EXPECT_EQ(m2.cents, 60);
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
}
