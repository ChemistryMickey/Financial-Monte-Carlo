#include "json.hpp"
#include <print>
#include "include/Logger.hpp"

fmc::Logger logger{"output/output.log"};

int main() {
    INFO("Logger successfully initialized");
}