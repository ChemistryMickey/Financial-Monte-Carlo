#pragma once

#ifdef UNIT_TESTING
#define TESTING_PUBLIC public:
#else
#define TESTING_PUBLIC private:
#endif