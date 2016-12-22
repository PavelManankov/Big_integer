#include "testing.h"

#include <cstdlib>
#include <cstdio>

size_t tests_run = 0;
size_t tests_fail = 0;

void warn_if_false_func(bool cond, char const* expr_str)
{
    if (!cond)
    {
        tests_fail++;
        printf("check %s failed\n", expr_str);
    }

    tests_run++;
}

void print_test_results()
{
    if (tests_run == 0)
        printf("no tests were run");
    else if (tests_fail == 0)
        printf("all %zu tests are run successfully\n", tests_run);
    else
        printf("%zu/%zu tests failed", tests_fail, tests_run);
}
