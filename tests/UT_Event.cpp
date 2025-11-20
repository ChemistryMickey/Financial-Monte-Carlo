#include "gtest/gtest.h"
#include "json.hpp"

#include "Event.hpp"

namespace fmc {
    struct Test_Event : public ::testing::Test {
        nlohmann::json config{
            {"event_val", 100},
            {"probability", 0},
            {"rng_seed", 42},
            {"scaling_factor", 1.1},
            {"duration", 0},
            {"cooldown", 10},
            {"knockdown", -0.05}
        };
        Event event{config};
    };

    TEST_F(Test_Event, init) {
        // Just confirm that things transferred properly
        EXPECT_EQ(event.effect_val, 100);
        EXPECT_EQ(event.probability.get_value(), 0);
        EXPECT_EQ(event.scaling_factor.get_value(), 1.1);
        EXPECT_EQ(event.duration, 0);
        EXPECT_EQ(event.cooldown, 10);
        EXPECT_EQ(event.knockdown.get_value(), -0.05);
    }

    TEST_F(Test_Event, update) {
        event.probability.set_value(0.1);

        event.update(1);
        EXPECT_NEAR(event.probability.get_value(), 0.1 * 1.1 * (1 + 1.0 / 365.0), 1e-6);
    }

    TEST_F(Test_Event, proc) {
        event.probability.set_value(100); // This should get clamped to 1
        EXPECT_EQ(event.probability.get_value(), 1);
        EXPECT_TRUE(event.occurred());

        event.probability.set_value(-100); // Should get clamped to 0
        EXPECT_EQ(event.probability.get_value(), 0);
        EXPECT_FALSE(event.occurred());
    }

    TEST_F(Test_Event, cooldown) {
        event.days_since_last_proc = 0; // Puts this on cooldown
        event.probability.set_value(1); // Set guaranteed natural proc

        EXPECT_FALSE(event.occurred());

        event.days_since_last_proc = 11; // Now off cooldown
        EXPECT_TRUE(event.occurred());
    }

    TEST_F(Test_Event, dot) {
        event.days_since_last_proc = 0; // Was recently triggered
        event.duration = 3; // Will DoT for 3 days
        // No natural prob necessary. Should dot proc.

        EXPECT_TRUE(event.occurred()); // Should be true from the DoT

        event.update(3);
        EXPECT_EQ(event.days_since_last_proc, 3); // DoT should be over and it should still be on cooldown
        EXPECT_FALSE(event.occurred());

        event.update(7); // It should now be off cooldown
        EXPECT_EQ(event.days_since_last_proc, 10);
        EXPECT_EQ(event.probability.get_value(), 0);
        EXPECT_FALSE(event.occurred()); // but the nat prob is still 0

        event.probability.set_value(1);
        EXPECT_TRUE(event.occurred()); // Natural proc
        EXPECT_EQ(event.days_since_last_proc, 0);
        event.probability.set_value(0); // Disable nat proc

        EXPECT_TRUE(event.occurred()); // DoT
        event.update(1);
        EXPECT_TRUE(event.occurred()); // DoT
        event.update(1);
        EXPECT_TRUE(event.occurred()); // DoT
        EXPECT_EQ(event.days_since_last_proc, 2);

        event.update(1);
        EXPECT_FALSE(event.occurred()); // Ended
        EXPECT_EQ(event.days_since_last_proc, 3);

        event.probability.set_value(1); // Enable nat proc
        event.update(1);
        EXPECT_FALSE(event.occurred()); // On cooldown
        EXPECT_EQ(event.days_since_last_proc, 4);
    }
}