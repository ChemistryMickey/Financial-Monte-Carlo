#include "json.hpp"
#include <print>
#include "include/Logger.hpp"

fmc::Logger logger{"output/output.log"};

struct Person {
    int a = 0;
};

int main() {
    INFO("Logger successfully initialized");
}