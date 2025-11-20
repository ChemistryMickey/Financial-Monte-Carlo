#pragma once

namespace fmc {
    struct Model {
        virtual void initialize() = 0;
        virtual void update(unsigned int) = 0;
    };
}