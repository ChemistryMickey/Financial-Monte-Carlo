#include "gtest/gtest.h"
#include <map>
#include <functional>
#include <iostream>

#include <rttr/type>
#include <rttr/registration>
#include <rttr/registration_friend>

#include "utils.hpp"

struct Loggable {
    RTTR_ENABLE();
    // This allows RTTR to access (and therefore we can log) private methods/properties
    RTTR_REGISTRATION_FRIEND
};

class Foo : public Loggable {
    // Note, this is private and I can still log it! Heck yeah.
    double a = 42;

    RTTR_ENABLE(Loggable);
    // This allows RTTR to access (and therefore we can log) private methods/properties
    RTTR_REGISTRATION_FRIEND
};

struct Bar : public Loggable {
    double c = 299792458;

    RTTR_ENABLE(Loggable);
    // This allows RTTR to access (and therefore we can log) private methods/properties. Indent is annoying tho
    RTTR_REGISTRATION_FRIEND
};


template <typename T>
void print_most_derived_class(T* obj) {
    rttr::type obj_type = rttr::type::get(*obj);

    std::cout << "Most derived class: " << obj_type.get_name() << "\n";
}

RTTR_REGISTRATION{
    rttr::registration::class_<Loggable>("Loggable");
    rttr::registration::class_<Foo>("Foo")
        .property("a", &Foo::a);
    rttr::registration::class_<Bar>("Bar")
        .property("c", &Bar::c);
}

struct TimeseriesLogger {
    std::map<std::string, std::reference_wrapper<Loggable>> loggables;
    std::vector<std::string> signals_to_log;
};

TEST(Test_loggable, registering) {
    Foo f{};
    Bar b{};
    TimeseriesLogger ts_logger{.loggables = {
        {"f", f},
        {"b", b}
    }, .signals_to_log = {
        "f.a",
        "b.c"
    }};
    print_most_derived_class(&f);

    for (auto& [key, _] : ts_logger.loggables) {
        std::cout << "Registered: " << key << "\n";
    }

    for (auto& signal : ts_logger.signals_to_log) {
        std::vector<std::string> bits = split(signal, '.');

        if (ts_logger.loggables.contains(bits[0])) {
            auto& obj = ts_logger.loggables.at(bits[0]).get();
            auto inst = rttr::instance(obj);

            // Get the type of the object in the map
            rttr::type obj_type = inst.get_derived_type();

            if (auto prop = obj_type.get_property(bits[1])) {
                std::cout << "Value of " << signal << ": "
                    << prop.get_value(obj).get_value<double>() << "\n";
            }
            else {
                std::cout << "Unknown type: " << obj_type.get_name() << "\n";
            }
        }
    }

    // Dude. Fuck yeah.
    // Now I just gotta make that split nonsense recursive and plumb this up in a production type.
}