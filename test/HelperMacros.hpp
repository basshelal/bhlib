#ifndef BHLIB_TEST_HELPERMACROS
#define BHLIB_TEST_HELPERMACROS

#include <catch2/catch_all.hpp>

#define CHECK_WITH_VARIABLE(variable, ...) \
    CHECKED_IF(__VA_ARGS__) {              \
        variable &= true;                  \
    }

#endif  // BHLIB_TEST_HELPERMACROS
