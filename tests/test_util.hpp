#ifndef LN_TESTS_TEST_UTIL_HPP
#define LN_TESTS_TEST_UTIL_HPP

#include <iostream>

namespace ln::test
{

inline int g_failures = 0;

inline void Check(bool condition, const char* description)
{
    if (!condition)
    {
        std::cerr << "FAILED: " << description << '\n';
        ++g_failures;
    }
}

}

#endif
