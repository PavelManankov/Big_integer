#include "big_integer.h"

#include <cassert>
#include <vector>

using namespace std;

static void trim_leading_zeros(vector<digit> &a)
{
    while (a.size()!=0 && a[a.size()-1] == 0)
        a.pop_back();
}

static void divide(big_integer const& delimoe, int delitel,big_integer& chastnoe,int& ostatok)
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

// a * 10^n
static big_integer mul_power_of_BASE(big_integer const& a, size_t n)
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
static bool shifted_less(big_integer const& a, big_integer const& b, size_t n)
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
static void shifted_sub(big_integer& minuend, big_integer const& subtrahend, size_t n)
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

static int guess_digit(big_integer& delimoeostatok, big_integer const& delitel, size_t n)
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

static void divide_school_book(big_integer &delimoe,big_integer const&delitel,big_integer &chastnoe)
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

big_integer::big_integer()
{}

big_integer::big_integer(int a)
{
    assert(a >= 0);

    while (a != 0)
    {
        digits.push_back(a % BASE);
        a /= BASE;
    }
}

big_integer::big_integer(std::string const& str)
{
    for (size_t i = 0; i != str.size(); ++i)
    {
        char c = str[i];
        assert(c >= '0' && c <= '9');
        *this = *this * big_integer(10) + big_integer(c - '0');
    }
}

digit big_integer::get_digit(size_t index) const
{
    if (index >= digits.size())
        return 0;

    return digits[index];
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

bool operator==(big_integer const& a, big_integer const& b)
{
    if (a.digits.size() != b.digits.size())
        return false;

    size_t m = a.digits.size();
    for(size_t i = 0; i < m; i++)
        if(a.digits[i] != b.digits[i])
             return false;

    return true;
}

bool operator!=(big_integer const& a, big_integer const& b)
{
    return !(a == b);
}

bool operator>(big_integer const&a,big_integer const&b)
{
    if (a.digits.size() != b.digits.size())
        return a.digits.size() > b.digits.size();

    for (size_t i = a.digits.size(); i != 0; --i)
    {
        if (a.digits[i - 1] != b.digits[i - 1])
            return a.digits[i - 1] > b.digits[i - 1];
    }

    return false;
}

bool operator<(big_integer const&a,big_integer const&b)
{

        return (b>a);

}

bool operator<=(big_integer const&a,big_integer const&b)
{

        return !(a>b);

}

bool operator>=(big_integer const& a, big_integer const& b)
{
    return !(b > a);
}