#ifndef TESTING_H
#define TESTING_H

#define warn_if_false(expr) warn_if_false_func((expr),#expr)

void warn_if_false_func(bool cond, char const* expr_str);
void print_test_results();

#endif // TESTING_H
