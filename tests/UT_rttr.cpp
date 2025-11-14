#include <rttr/registration>
#include "gtest/gtest.h"
#include <print>

struct Foo {
    Foo() {}
    int data;
    double func(double b) { return b + 1; }
};

RTTR_REGISTRATION{
    rttr::registration::class_<Foo>("Foo")
        .constructor<>()
        .property("data", &Foo::data)
        .method("func", &Foo::func);
}

TEST(Test_rttr, manual_registration) {
    Foo foo{};
    rttr::type t = rttr::type::get(foo);
    for (auto& prop : t.get_properties()) {
        ASSERT_EQ(prop.get_name(), "data");
    }
    for (auto& meth : t.get_methods()) {
        ASSERT_EQ(meth.get_name(), "func");
    }
}

TEST(Test_rttr, set_values) {
    Foo foo{};
    rttr::type t = rttr::type::get(foo);
    t.get_property("data").set_value(foo, 808);

    ASSERT_EQ(foo.data, 808);
}