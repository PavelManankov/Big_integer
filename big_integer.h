#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <vector>
#include <string>
#include <istream>
#include <ostream>

const int BASE = 2000000;
typedef int digit;
typedef long long double_digit;

struct big_integer
{
    big_integer();
    big_integer(int a);
    explicit big_integer(std::string const& str);

    digit get_digit(size_t index) const;

    std::vector<digit> digits;
    bool sign;
};

std::istream& operator>>(std::istream& is, big_integer&);
std::ostream& operator<<(std::ostream& os, big_integer);

big_integer operator+(big_integer const& a,big_integer const& b);
big_integer operator-(big_integer const& a,big_integer const& b);
big_integer operator*(big_integer const& a, int b);
big_integer operator*(big_integer const& a,big_integer const& b);
big_integer operator/(big_integer const& delimoe, int delitel);
int operator%(big_integer const& delimoe, int delitel);
big_integer operator/(big_integer delimoe, big_integer const& delitel);
big_integer operator%(big_integer const& delimoe, big_integer const& delitel);

big_integer& operator+=(big_integer& a,big_integer const& b);
big_integer& operator-=(big_integer& a,big_integer const& b);
big_integer& operator*=(big_integer& a,big_integer const& b);
big_integer& operator*=(big_integer& a, int b);
big_integer& operator/=(big_integer& a,big_integer const& b);
big_integer& operator/=(big_integer& a, int b);
big_integer& operator%=(big_integer& a, big_integer const& b);
big_integer& operator%=(big_integer& a, int b);
big_integer& operator++(big_integer& a);
big_integer operator++(big_integer& a, int);
big_integer& operator--(big_integer& a);
big_integer operator--(big_integer& a, int);

big_integer operator-(big_integer const& a);

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);

big_integer number_of_permutations(int n);
big_integer number_of_partial_permutations (int n, int k);
big_integer number_of_combinations(int n,int k);

big_integer gcd_euclid(big_integer a, big_integer b);
big_integer lcm_euclid(big_integer a, big_integer b);
#endif // BIG_INTEGER_H
