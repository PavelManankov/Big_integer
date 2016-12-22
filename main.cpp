#include <algorithm>
#include <string>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <chrono>
#include <iostream>

using namespace std;

const int BASE = 2000000;
typedef int digit;
typedef long long double_digit;

void trim_leading_zeros(vector<digit> &a)
{
    while (a.size()!=0 && a[a.size()-1] == 0)
        a.pop_back();
}


struct big_integer
{
    big_integer()
    {}

    big_integer(int a)
    {
        assert(a >= 0);

        while (a != 0)
        {
            digits.push_back(a % BASE);
            a /= BASE;
        }
    }

    big_integer(std::string const& str);

    digit get_digit(size_t index) const
    {
        if (index >= digits.size())
            return 0;

        return digits[index];
    }

    std::vector<digit> digits;
};

big_integer operator+(big_integer const& a,big_integer const& b)
{
    big_integer c;
    size_t m;
    digit carry=0;

    if (a.digits.size()<b.digits.size())
         m=b.digits.size();
    else
         m=a.digits.size();

    for (size_t i=0;i<m;i++)
    {
        double_digit sum=a.get_digit(i)+b.get_digit(i)+carry;
        c.digits.push_back(sum%BASE);
        carry=sum/BASE;
    }
    if (carry!=0)
        c.digits.push_back(carry);
    return c;
}
bool operator==(big_integer const&a,big_integer const&b)
{
    if (a.digits.size()!=b.digits.size())
        return false;
    size_t m=a.digits.size();
    for(size_t i=0;i<m;i++)
        if(a.digits[i]!=b.digits[i])
             return false;

    return true;
}

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

#define warn_if_false(expr) warn_if_false_func((expr),#expr)

bool operator>(big_integer const&a,big_integer const&b)
{
    if(a.digits.size()==b.digits.size())
        {

        for(size_t i=a.digits.size();i!=0;)
        {
           if (a.digits[i-1]==b.digits[i-1])
               --i;
            else

               return a.digits[i-1]>b.digits[i-1];

        }
        return false;

        }
        return a.digits.size()>b.digits.size();


}
bool operator<(big_integer const&a,big_integer const&b)
{

        return (b>a);

}

bool operator <=(big_integer const&a,big_integer const&b)
{

        return !(a>b);

}

big_integer operator-(big_integer const&a,big_integer const&b)
{
    big_integer c;
    digit take=0;

    for(size_t i=0;i<a.digits.size();++i)
    {
        {
            double_digit minus=a.digits[i]-b.get_digit(i)+take;

            if(minus<0)
            {
                 c.digits.push_back(BASE+minus);
                 take=-1;
            }
            else
            {
                c.digits.push_back(minus);
                take=0;
            }

        }
   // while ((t-m)>0);
    }
    trim_leading_zeros(c.digits);
    return c;
}


/*big_integer operator*(big_integer const&a,big_integer

const&b)
{
    big_integer c;
    int carry =0;
    for(size_t i=0;i!=a.digits.size();++i)
    {
        int us=b*a.get_digit(i)+carry;
        c.digits.push_back(us%10);
        carry=us/10;
        }
    if(carry!=0)
    {
        c.digits.push_back(carry);
    }

    return c;
}
*/
big_integer operator*(big_integer const&a, int b)
{
    assert(b >= 0 && b < BASE);

    digit carry = 0;
    big_integer c;
    if(b==0)
        return c;
    for(size_t i=0; i!=a.digits.size();++i)
    {
        double_digit us=double_digit(b)*a.digits[i]+carry;
        c.digits.push_back(us%BASE);
        carry=us/BASE;

    }
    if(carry!=0)
    c.digits.push_back(carry);
    return c;
}

//ac
/*
big_integer operator*(big_integer const&a,big_integer const&b)
{
   big_integer c;

//aieacaoaeunoai ?aca?ae?iaaiey aeeiiu
   //   99
   //   99
   //  891
   // 891
   // 9801
   99*99
   9801
   999*999
   998001
   9999*9999
   99980001
   99999*99999
   9999800001
   (10^n - 1)(10^m - 1) = 10^(n+m) - (10^n + 10^m - 1)
   10^n + 10^m - 1 >= 1
   10^n + 10^m >= 2


   c.digits.resize(a.digits.size()+b.digits.size());

   for (int i=0;i<a.digits.size();++i)
   {
       for (int j=0;j<b.digits.size();++j)
       {
           c.digits[i+j]+=a.digits[i]*b.digits[j];

           int k=i+j;
           while ((c.digits[k])>=10)
           {
              int carry=c.digits[k]/10;
               c.digits[k]=c.digits[k]%10;
               c.digits[k+1]+=carry;
               k++;
           }
       }
   }
   trim_leading_zeros(c.digits);
   return c;
}

*/

big_integer operator*(big_integer const&a,big_integer const&b)
{
   big_integer c;
   c.digits.resize(a.digits.size()+b.digits.size());

   for(size_t i=0;i<a.digits.size();++i)
   {
       digit carry=0;

       for(size_t j=0;j<b.digits.size();++j)
       {
           double_digit tmp = double_digit(a.digits[i]) * b.digits[j] + c.digits[i+j] + carry;
           c.digits[i+j] = tmp % BASE;
           carry=tmp / BASE;
       }
       //assert - i?iaa?yao oneiaea eioi?ia a iai anou
       // true - ie?aai ia aaeaao, false - auaaao ioeaeo
       assert(c.digits[i+b.digits.size()]==0);
       c.digits[i+b.digits.size()]=carry;
   }
   trim_leading_zeros(c.digits);
   return c;
}
//  99
//   9
// 891

big_integer::big_integer(std::string const& str)
{
    for (size_t i = 0; i != str.size(); ++i)
    {
        char c = str[i];
        assert(c >= '0' && c <= '9');
        *this = *this * big_integer(10) + big_integer(c - '0');
    }
}



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



void divide(big_integer const& delimoe, int delitel,big_integer& chastnoe,int& ostatok)
{
    digit carry=0;

    chastnoe.digits.resize(delimoe.digits.size());
    for(size_t i=delimoe.digits.size();i!=0;--i)
    {
        double_digit t=double_digit(carry)*BASE+delimoe.digits[i-1];
        chastnoe.digits[i-1]=t/delitel;
          carry =t%delitel;
    }
     trim_leading_zeros(chastnoe.digits);
      ostatok=carry;
  }

/*
  123456 |_ 4444
  -8888     00002


*/

// a * 10^n
big_integer mul_power_of_BASE(big_integer const& a, size_t n)
{
    big_integer result;

    for (size_t i = 0; i != n; ++i)
        result.digits.push_back(0);

    for (size_t i = 0; i != a.digits.size(); ++i)
        result.digits.push_back(a.digits[i]);

    return result;
}

// a      [1 2 x x x x x]      size=7
// b                [1 2]      size=2     n=5
// b*10^5 [1 2 0 0 0 0 0]

// i={7..5}
bool shifted_less(big_integer const& a, big_integer const& b, size_t n)
{
    if (a.digits.size() != (b.digits.size() + n))
        return a.digits.size() < (b.digits.size() + n);

    for (size_t i = a.digits.size(); i != n;)
    {
        if (a.digits[i-1] == b.digits[i-1-n])
            --i;
        else
            return a.digits[i-1] < b.digits[i-1-n];
    }

    return false;
}


//    2341234812634812764218461283746128347612384761238476123874612834612847162348712364
//                                                         123
//                                                           100000000000000000000000000
//    2341234812634812764218461283746128347612384761238476111574612834612847162348712364
void shifted_sub(big_integer& minuend, big_integer const& subtrahend, size_t n)
{
    //minuend = minuend - mul_power_of_10(subtrahend, n);

    digit take=0;
    size_t i=0;
    for(;i<subtrahend.digits.size();++i)
    {
        double_digit minus=minuend.digits[i + n]-subtrahend.digits[i]+take;

        if(minus<0)
        {
            minus += BASE;
            take=-1;
        }
        else
        {
            take=0;
        }

        minuend.digits[i + n] = minus;
    }
    if (take)
    {
        while (minuend.digits[i + n] == 0)
        {
            minuend.digits[i + n] = BASE - 1;
            ++i;
        }
        --minuend.digits[i + n];
    }
    trim_leading_zeros(minuend.digits);
}

int guess_digit_1(big_integer& delimoeostatok, big_integer const& delitel, size_t n)
{
    size_t dn = delitel.digits.size();
    double_digit d2 = double_digit(delitel.digits[dn - 1]) * BASE
                    + delitel.digits[dn - 2];

    double_digit r3 = double_digit(delimoeostatok.get_digit(dn + n))     * BASE * BASE
                    + double_digit(delimoeostatok.get_digit(dn + n - 1)) * BASE
                    + delimoeostatok.get_digit(dn + n - 2);

    int qe = r3 / d2;
    assert(qe >= 0 && qe <= BASE);
    if (qe != 0)
        --qe;

    assert(!shifted_less(delimoeostatok,delitel * qe,n));
    shifted_sub(delimoeostatok, delitel * qe, n);

    if (!shifted_less(delimoeostatok,delitel,n))
    {
        ++qe;
        shifted_sub(delimoeostatok,delitel,n);
    }

    return qe;
}

int guess_digit_2(big_integer &delimoeostatok, big_integer const&delitel, size_t n)
{
    int i=0;

    while (!shifted_less(delimoeostatok,delitel,n))
    {
        i++;
        shifted_sub(delimoeostatok,delitel,n);
    }

    return i;
}

int guess_digit(big_integer& delimoeostatok, big_integer const& delitel, size_t n)
{
    return guess_digit_1(delimoeostatok, delitel, n);
/*    big_integer copy1 = delimoeostatok;
    int g1 = guess_digit_1(copy1, delitel, n);

    big_integer copy2 = delimoeostatok;
    int g2 = guess_digit_2(copy2, delitel, n);

    assert(g1 == g2);
    delimoeostatok = copy1;

    return g1;
    */

}

void divide_school_book(big_integer &delimoe,big_integer const&delitel,big_integer &chastnoe)
{
    if (delitel.digits.size() == 1)
    {
        int remainder;
        divide(delimoe, delitel.digits[0], chastnoe, remainder);
        delimoe = big_integer(remainder);
        return;
    }

    chastnoe.digits.resize(delimoe.digits.size()-delitel.digits.size()+1);

    for(size_t i=chastnoe.digits.size(); i!=0; i--)
    {
        chastnoe.digits[i-1] = guess_digit(delimoe,delitel,i-1);
    }

    trim_leading_zeros(chastnoe.digits);
}

big_integer operator/(big_integer const& delimoe,int delitel)
{
   big_integer chastnoe;
            int ostatok;
            //divide

            divide(delimoe, delitel, chastnoe, ostatok);
    return chastnoe;
}

int operator%(big_integer const& delimoe, int delitel)
{

    big_integer chastnoe;
             int ostatok;
             //divide
    divide(delimoe, delitel, chastnoe, ostatok);
     return ostatok;
}

void print(big_integer val)
{
    if (val.digits.size() == 0)
        printf("0");
    else
    {
        std::string result;

        while (val.digits.size() != 0)
        {
            result.push_back((val % 10) + '0');
            val = val / 10;
        }

        for (size_t i = 0; i != (result.size() / 2); ++i)
            std::swap(result[i], result[result.size() - i - 1]);

        printf("%s", result.c_str());
    }
    printf("\n");
}

void divide(big_integer const& delimoe, big_integer delitel, big_integer& chastnoe)
{
    big_integer left;
    big_integer right = delimoe+big_integer("1");

    for(;;)
    {
       if(left==right)
       {
            chastnoe=left-big_integer("1");
            break;
       }
       big_integer index=(right+left)/2;
       big_integer element=index*delitel;
       if (element==delimoe)
       {
           chastnoe = index;
            break;
       }
       if(element<delimoe)
           left=index+big_integer("1");
       else
           right=index;

    }

}

big_integer operator/(big_integer delimoe, big_integer const& delitel)
{
    big_integer chastnoe;

    divide_school_book(delimoe,delitel,chastnoe);
    return chastnoe;

    /*
    big_integer left;
    big_integer right = delimoe+big_integer("1");

    for(;;)
    {
        if(left==right)
           return left - big_integer("1");

        big_integer index=(right+left)/2;
        big_integer element=index*delitel;
        if (element==delimoe)
           return index;

        if(element<delimoe)
           left=index+big_integer("1");
        else
           right=index;
    }
*/
}

big_integer operator%(big_integer const& delimoe, big_integer const& delitel)
{
    return delimoe - (delimoe / delitel) * delitel;
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

    if (tests_run == 0)
        printf("no tests were run");
    else if (tests_fail == 0)
        printf("all %zu tests are run successfully\n", tests_run);
    else
        printf("%zu/%zu tests failed", tests_fail, tests_run);
}
