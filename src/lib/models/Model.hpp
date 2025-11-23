#pragma once

namespace fmc {
    struct Model {
        virtual void initialize() = 0;
        virtual void update(unsigned int) = 0;
        virtual void environment(unsigned int) = 0;
        virtual void yearly_update() = 0;

        virtual bool should_terminate() = 0;
    };
}