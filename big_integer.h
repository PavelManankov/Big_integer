#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <vector>
#include <string>

const int BASE = 2000000;
typedef int digit;
typedef long long double_digit;

struct big_integer
{
    big_integer();
    big_integer(int a);
    big_integer(std::string const& str);

    digit get_digit(size_t index) const;

    std::vector<digit> digits;
};

void print(big_integer val);

big_integer operator+(big_integer const& a,big_integer const& b);
big_integer operator-(big_integer const& a,big_integer const& b);
big_integer operator*(big_integer const& a, int b);
big_integer operator*(big_integer const& a,big_integer const& b);
big_integer operator/(big_integer const& delimoe, int delitel);
int operator%(big_integer const& delimoe, int delitel);
big_integer operator/(big_integer delimoe, big_integer const& delitel);
big_integer operator%(big_integer const& delimoe, big_integer const& delitel);

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);

#endif // BIG_INTEGER_H
