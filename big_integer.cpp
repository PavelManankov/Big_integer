#include "big_integer.h"

#include <cassert>
#include <vector>

using namespace std;

static void trim_leading_zeros(big_integer &a)
{
    while (a.digits.size()!=0 && a.digits[a.digits.size()-1] == 0)
        a.digits.pop_back();
    if (a.digits.size()==0)
        a.sign = false;
}

static void divide(big_integer const& delimoe, int delitel,big_integer& chastnoe,int& ostatok)
{
    assert(delitel > 0 && delitel < BASE);

    digit carry=0;

    chastnoe.digits.resize(delimoe.digits.size());
    chastnoe.sign = delimoe.sign;
    for(size_t i=delimoe.digits.size();i!=0;--i)
    {
        double_digit t=double_digit(carry)*BASE+delimoe.digits[i-1];
        chastnoe.digits[i-1]=t/delitel;
          carry =t%delitel;
    }
    trim_leading_zeros(chastnoe);

    if(delimoe.sign)
        ostatok = -carry;
    else
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
    if (b.digits.size() == 0)
        return false;

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
    trim_leading_zeros(minuend);
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
    assert(!delitel.sign);

    if (delitel.digits.size() == 1)
    {
        int remainder;
        divide(delimoe, delitel.digits[0], chastnoe, remainder);
        delimoe = big_integer(remainder);
        return;
    }

    chastnoe.sign = delimoe.sign;
    chastnoe.digits.resize(delimoe.digits.size()-delitel.digits.size()+1);

    for(size_t i=chastnoe.digits.size(); i!=0; i--)
    {
        chastnoe.digits[i-1] = guess_digit(delimoe,delitel,i-1);
    }

    trim_leading_zeros(chastnoe);
}

big_integer::big_integer()
{
    sign = false;
}

big_integer::big_integer(int a)
{
    if (a < 0)
    {
        a = -a;
        sign = true;
    }
    else
        sign = false;

    while (a != 0)
    {
        digits.push_back(a % BASE);
        a /= BASE;
    }
}

big_integer::big_integer(std::string const& str)
{
    sign = false;
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

std::ostream& operator<<(std::ostream& os, big_integer val)
{
    if (val.sign)
    {
        os << '-';
        val = -val;
    }

    if (val.digits.size() == 0)
        os << '0';
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

        os << result;
    }

    return os;
}

static bool abs_less(big_integer const& a, big_integer const& b)
{
    if (a.digits.size() != b.digits.size())
        return a.digits.size() < b.digits.size();

    for (size_t i = a.digits.size(); i != 0; --i)
    {
        if (a.digits[i - 1] != b.digits[i - 1])
            return a.digits[i - 1] < b.digits[i - 1];
    }

    return false;
}

static big_integer abs_sum(big_integer const& a,big_integer const& b)
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

static big_integer abs_difference(big_integer const&a,big_integer const&b)
{
    assert(!abs_less(a, b));
    big_integer c;
    digit take=0;

    for(size_t i=0;i<a.digits.size();++i)
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
    assert(take == 0);
    trim_leading_zeros(c);
    return c;
}

big_integer operator+(big_integer const& a,big_integer const& b)
{
    big_integer result;
    if (a.sign == b.sign)
    {
        result = abs_sum(a, b);
        result.sign = a.sign;
    }
    else
    {
        if (abs_less(a, b))
        {
            result = abs_difference(b, a);
            result.sign = b.sign;
        }
        else
        {
            result = abs_difference(a, b);
            result.sign = a.sign;
        }
    }
    return result;
}

big_integer operator-(big_integer const&a,big_integer const&b)
{
    return a + -b;
}

big_integer mul_long_short(big_integer const&a, int b)
{
    assert(b > 0 && b < BASE);

    digit carry = 0;
    big_integer c;

    c.digits.resize(a.digits.size() + 1);
    size_t i;
    for(i=0; i!=a.digits.size();++i)
    {
        double_digit us=double_digit(b)*a.digits[i]+carry;
        c.digits[i] = us%BASE;
        carry=us/BASE;

    }
    c.digits[i] = carry;

    trim_leading_zeros(c);
    return c;
}

big_integer operator*(big_integer const&a, int b)
{
    if (a.digits.size()==0||b==0)
        return big_integer();

    if (b>0 && b < BASE)
        return mul_long_short(a, b);

    if(b<0 && -b < BASE)
    {
        big_integer result = mul_long_short(a, -b);// мы к отрицательному еще добавляем один минус чтобы его сделать положительеым для assert
        result.sign = !result.sign;
        return result;
    }

    return a * big_integer(b);
}

big_integer operator*(big_integer const&a,big_integer const&b)
{
   big_integer c;
   c.digits.resize(a.digits.size()+b.digits.size());
   c.sign = a.sign^b.sign;

   for(size_t i=0;i<a.digits.size();++i)
   {
       digit carry=0;

       for(size_t j=0;j<b.digits.size();++j)
       {
           double_digit tmp = double_digit(a.digits[i]) * b.digits[j] + c.digits[i+j] + carry;
           c.digits[i+j] = tmp % BASE;
           carry=tmp / BASE;
       }
       assert(c.digits[i+b.digits.size()]==0);
       c.digits[i+b.digits.size()]=carry;
   }
   trim_leading_zeros(c);
   return c;
}

big_integer operator/(big_integer const& delimoe,int delitel)
{
    assert(delitel != 0);

    big_integer chastnoe;
    int ostatok;

    if (delitel>0 && delitel < BASE)
    {
        divide(delimoe, delitel, chastnoe, ostatok);
        return chastnoe;
    }

    if(delitel<0 && -delitel < BASE)
    {
        divide(-delimoe, -delitel, chastnoe, ostatok);
        return chastnoe;
    }

    return delimoe / big_integer(delitel);
}

int operator%(big_integer const& delimoe, int delitel)
{
    big_integer chastnoe;
    int ostatok;

    if(delitel<0)
        divide(-delimoe, -delitel, chastnoe, ostatok);// минус меняет знак
    else
        divide(delimoe, delitel, chastnoe, ostatok);
    return ostatok;
}

big_integer operator/(big_integer delimoe, big_integer const& delitel)
{
    assert(delitel != 0);

    big_integer chastnoe;

    if (delitel.sign)
    {
        delimoe = -delimoe;
        divide_school_book(delimoe,-delitel,chastnoe);
    }
    else
        divide_school_book(delimoe,delitel,chastnoe);

    return chastnoe;
}

big_integer operator%(big_integer const& delimoe, big_integer const& delitel)
{
    return delimoe - (delimoe / delitel) * delitel;
}

big_integer& operator+=(big_integer& a,big_integer const& b)
{
    a = a + b;
    return a;
}

big_integer& operator-=(big_integer& a,big_integer const& b)
{
    a = a - b;
    return a;
}

big_integer& operator*=(big_integer& a,big_integer const& b)
{
    a = a * b;
    return a;
}

big_integer& operator*=(big_integer& a,int b)
{
    a = a * b;
    return a;
}

big_integer& operator/=(big_integer& a,big_integer const& b)
{
    a = a / b;
    return a;
}

big_integer& operator/=(big_integer& a,int b)
{
    a = a / b;
    return a;
}

big_integer& operator%=(big_integer& a, big_integer const& b)
{
    a = a % b;
    return a;
}

big_integer& operator%=(big_integer& a, int b)
{
    a = a % b;
    return a;
}

big_integer& operator++(big_integer& a)
{
    a = a + 1;
    return a;
}

big_integer operator++(big_integer& a, int)
{
    big_integer old = a;
    ++a;
    return old;
}

big_integer& operator--(big_integer& a)
{
    a = a - 1;
    return a;
}

big_integer operator--(big_integer& a, int)
{
    big_integer old = a;
    --a;
    return old;
}

big_integer operator-(big_integer const& a)
{
    big_integer temp = a;
    if(temp.digits.size()!=0)
        temp.sign = !temp.sign;
    return temp;
}

bool operator==(big_integer const& a, big_integer const& b)
{
    if(a.sign!=b.sign)
        return false;
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

bool operator<(big_integer const& a, big_integer const& b)
{
    if  (a.sign && b.sign)
        return abs_less(b, a);
    else if(!a.sign && b.sign)
        return false;
    else if(a.sign && !b.sign)
        return true;
    else
        return abs_less(a, b);
}

bool operator>(big_integer const& a, big_integer const& b)
{
    return b < a;
}

bool operator<=(big_integer const& a,big_integer const& b)
{
    return !(b < a);
}

bool operator>=(big_integer const& a, big_integer const& b)
{
    return !(a < b);
}
