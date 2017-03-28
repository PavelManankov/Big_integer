#include <algorithm>
#include <string>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <chrono>
#include <sstream>
#include <iostream>

#include "testing.h"
#include "big_integer.h"

using namespace std;


big_integer factorial (big_integer const&a)
{
    big_integer c("1");

    for(big_integer i("1");i<=a;i=i+big_integer("1"))
    {
        c=c*i;

    }
    return c;
}



big_integer step (big_integer const&a, big_integer const& b)
{
    big_integer c("1");

    for(big_integer i("1");i<=b;i=i+big_integer("1"))
    {
        c=c*a;
    }
return c;
}


big_integer fibonacci(big_integer const&a)
{
    big_integer prev_prev("0");
    big_integer prev("1");

    for (big_integer i("1"); i<=a; i=i+big_integer("1"))
    {
        big_integer current=prev_prev+prev;
                    prev_prev = prev;
                    prev = current;

    }
    return prev_prev;
}


big_integer pow(big_integer k, unsigned n)
{
    big_integer result("1");

    unsigned current_bit = 0;

    while (n != 0)
    {
        unsigned bit_mask = 1 << current_bit;
        if ((n & bit_mask) != 0)
        {
            result = result * k;
        }

        k = k * k;
        n = n & ~bit_mask;
        ++current_bit;
    }

    return result;
}



void bench()
{
    big_integer a("347123487612384761238476214872361472316481237648217461283476213847123648712"
                  "762348721647231647231542716521765231746512765127451274512374615237421534762"
                  "123476128476218476128476213847612348712634872316487126482713648123746218374"
                  "123237462813641874623874621847621384612384761238462343214172346271657652347");

    big_integer b("1234123412364521466421734123412341234");

    for (size_t i = 0; i != 10000; ++i)
        a / b;
}

int main()
{
    auto start_time = std::chrono::high_resolution_clock::now();
    bench();
    auto end_time = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " ms" << std::endl;

    warn_if_false((big_integer("456123")/big_integer("462"))==big_integer("987"));
    warn_if_false((big_integer("123456")/5)==big_integer("24691"));
    warn_if_false((big_integer("11111")/9)==big_integer("1234"));
    warn_if_false((pow(big_integer("2"), 10))==big_integer("1024"));
    warn_if_false(big_integer("99") + big_integer("1") == big_integer("100"));
    warn_if_false(big_integer("0099") + big_integer("001") == big_integer("100"));
    warn_if_false(big_integer("99") < big_integer("100"));
    warn_if_false(!(big_integer("5") > big_integer("5")));
    warn_if_false(!(big_integer("5") < -big_integer("5")));
    warn_if_false(-big_integer("5") < -big_integer("4"));
    warn_if_false(-big_integer("5") > -big_integer("6"));
    warn_if_false(-big_integer("5") < -big_integer("2"));
    warn_if_false((big_integer("0") * -1)==big_integer("0"));
    warn_if_false(big_integer("100") > big_integer("99"));
    warn_if_false(big_integer("321") > big_integer("123"));
    warn_if_false(big_integer("123") < big_integer("321"));
    warn_if_false((big_integer("123") * big_integer(5) )==big_integer("615"));
    warn_if_false((big_integer("123") * 0 )==big_integer("0"));
    warn_if_false(big_integer("123") * big_integer("123")==big_integer("15129"));
    warn_if_false(big_integer("456123") - big_integer("46200")==big_integer("409923"));
    warn_if_false(big_integer("1234") / big_integer("5")==big_integer("246"));
    warn_if_false(big_integer("412341234") / big_integer("23451")==big_integer("17583"));
    warn_if_false(big_integer("100000000000000")
                / big_integer("10000")
               == big_integer(    "10000000000"));
    warn_if_false(big_integer("12387641827346128734612983764") * big_integer(BASE) * big_integer(BASE)
                / big_integer("12387641827346128734612983764")
               == big_integer(BASE) * big_integer(BASE));

    big_integer a = 100;

    warn_if_false((a-=99)==big_integer("1"));
    warn_if_false((a+=99)==big_integer("100"));
    warn_if_false((a/=99)==big_integer("1"));
    warn_if_false((a*=99)==big_integer("99"));
    warn_if_false((a/=10)==big_integer("9"));
    warn_if_false((a%=5)==big_integer("4"));
    warn_if_false((++a)==big_integer("5"));
    warn_if_false((a++)==big_integer("5"));
    warn_if_false((--a)==big_integer("5"));
    warn_if_false((a--)==big_integer("5"));
    warn_if_false(big_integer(-5) == -big_integer(5));

    //тесты покрытия
    warn_if_false(-big_integer("123") * big_integer("123")==-big_integer("15129"));
    warn_if_false(big_integer("123") + -big_integer("124")==-big_integer("1"));
    warn_if_false(big_integer("124") + -big_integer("123")==big_integer("1"));
    warn_if_false(-big_integer("123") + -big_integer("123")==-big_integer("246"));
    warn_if_false(big_integer("123")%big_integer("60")==big_integer("3"));
    warn_if_false(big_integer("123")*big_integer("0")==big_integer("0"));
    warn_if_false(big_integer("123")*big_integer(-123)==-big_integer("15129"));

    big_integer l = 100;
    big_integer r = 100;

    warn_if_false((l+=r)==big_integer("200"));
    warn_if_false((l-=r)==big_integer("100"));
    warn_if_false((l/=r)==big_integer("1"));
    warn_if_false((l*=r)==big_integer("100"));
    warn_if_false((l%=r)==big_integer("0"));
    warn_if_false(-big_integer("1") < big_integer("1"));
    warn_if_false(big_integer("123")*(-123)==-big_integer("15129"));
    warn_if_false(big_integer("1")*(123123123)==big_integer("123123123"));
    warn_if_false(big_integer("1231")+big_integer("123")==big_integer("1354"));
    warn_if_false(-big_integer("412341234") / -big_integer("23451")==big_integer("17583"));
    warn_if_false(-big_integer("412341234") / big_integer("23451")==-big_integer("17583"));
    warn_if_false(-big_integer("412341234") / big_integer("23451")==-big_integer("17583"));
    warn_if_false(-big_integer("33") /11==-big_integer("3"));
    warn_if_false(-big_integer("33") /-11==big_integer("3"));
    warn_if_false(big_integer("33") /-11==-big_integer("3"));
    warn_if_false(33/-big_integer("11") ==-big_integer("3"));
    warn_if_false(-big_integer("123")%big_integer("60")==-big_integer("3"));
    warn_if_false(-big_integer("123")%(60)==-big_integer("3"));
    warn_if_false(-big_integer("123")%(-60)==big_integer("3"));
    warn_if_false(-big_integer("123") != big_integer("123"));
    warn_if_false(big_integer("123") != big_integer("1234"));
    warn_if_false(big_integer("123") >= big_integer("122"));
    warn_if_false(big_integer("122") <= big_integer("123"));
    warn_if_false(big_integer("20") != big_integer("200000000"));
    warn_if_false(big_integer("0") / big_integer("2")==(big_integer("0")));
    warn_if_false(big_integer("30000000") / 10000000 == (big_integer("3")));
    warn_if_false(big_integer("2000000") - big_integer("1999999") == (big_integer("1")));
    warn_if_false(big_integer("4000000000000") - big_integer("3999999999999") == (big_integer("1")));
    warn_if_false(big_integer("1999999") + big_integer("1") == (big_integer("2000000")));
    warn_if_false(big_integer("1") + big_integer("3999999999999") == (big_integer("4000000000000")));
    warn_if_false(big_integer("4000000000000") / big_integer("6000000") == (big_integer("666666")));
    warn_if_false(-big_integer("412341212123434") / big_integer("23412312351")==-big_integer("17612"));
    warn_if_false(big_integer("-1") == -big_integer("1"));
    warn_if_false(big_integer("-0") == big_integer("0"));

    warn_if_false(number_of_permutations(0)==1);
    warn_if_false(number_of_permutations(1)==1);
    warn_if_false(number_of_permutations(2)==2);
    warn_if_false(number_of_permutations(7)==5040);
    warn_if_false(number_of_partial_permutations(9,5)==(5*6*7*8*9));
    warn_if_false(number_of_combinations(5, 2)==10);
    warn_if_false(number_of_combinations(6, 2)==15);
    warn_if_false(number_of_combinations(0, 0)==1);
    warn_if_false(number_of_combinations(5, 5)==1);
    warn_if_false(number_of_combinations(5, 0)==1);
    {
        std::stringstream ss;
        ss << big_integer("100");
        warn_if_false(ss.str() == "100");
    }
    {
        std::stringstream ss;
        ss << big_integer("00100");
        warn_if_false(ss.str() == "100");
    }
    {
        std::stringstream ss;
        ss << -big_integer("0");
        warn_if_false(ss.str() == "0");
    }
    {
        std::stringstream ss;
        ss << -big_integer(5);
        warn_if_false(ss.str() == "-5");
    }

    print_test_results();
}
