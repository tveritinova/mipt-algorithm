//
//  main.cpp
//  big_int
//
//  Created by Евгения Тверитинова on 20.04.15.
//  Copyright (c) 2015 Евгения Тверитинова. All rights reserved.
//

#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
//#include <math.h>

int deg = 10000;

int pow(int a, int b)
{
    int res = 1;
    for (int i = 1; i <= b; i++)
    {
        res *= a;
    }
    return res;
}

int log10(int a)
{
    int counter_deg = 0;
    int b = 1;
    while (b < a)
    {
        b *= 10;
        ++counter_deg;
    }
    if (b == a)
    {
        return counter_deg;
    }
    else
    {
        return -1;
    }
}

class big_int
{
public:
    big_int() {}
    
    ~big_int()
    {
        for (int i = 0; i < this->size(); i++)
        {
            this->pop_back();
        }
    }
    
    big_int(int a)
    {
        int t = 1;
        if (a < 0)
        {
            sign = 1;
        }
        else
        {
            sign = 0;
        }
        if (a == 0)
        {
            body.push_back(a);
        }
        while (a / t != 0)
        {
            body.push_back((a % (t * deg) - a % t) / t);
            t *= deg;
        }
    }
    
    big_int(const big_int& a)
    {
        for (int i = 0; i < a.size(); i++)
        {
            body.push_back(a[i]);
        }
        sign = a.get_sign();
    }
    
    int operator [] (const int& a) const
    {
        return body[a];
    }
    
    big_int operator = (const big_int& a)
    {
        for (int i = 0; i < std::min(this->size(), a.size()); i++)
        {
            body[i] = a[i];
        }
        if (this->size() < a.size())
        {
            for (int i = this->size(); i < a.size(); i++)
            {
                this->push_back(a[i]);
            }
            return *this;
        }
        int size = this->size();
        if (this->size() > a.size())
        {
            for (int i = a.size(); i < size; i++)
            {
                this->pop_back();
            }
            return *this;
        }
        return *this;
    }
    
    big_int operator + (const big_int& a) const
    {
        big_int* res = new big_int;
        int size;
        int next = 0;
        int h;
        if (a.size() > this->size())
        {
            size = this->size();
        }
        else
        {
            size = a.size();
        }
        for (int i = 0; i < size; i++)
        {
            if (a[i] + (*this)[i] + next >= deg)
            {
                res->push_back(a[i] + (*this)[i] + next - deg);
                next = 1;
            }
            else
            {
                h = a[i] + (*this)[i] + next;
                res->push_back(h);
                next = 0;
            }
        }
        if (a.size() < this->size())
        {
            for (int i = size; i < this->size(); i++)
            {
                if ((*this)[i] + next >= deg)
                {
                    res->push_back((*this)[i] + next - deg);
                    next = 1;
                }
                else
                {
                    h = (*this)[i] + next;
                    res->push_back(h);
                    next = 0;
                }
            }
        }
        if (a.size() > this->size())
        {
            for (int i = size; i < a.size(); i++)
            {
                if (a[i] + next >= deg)
                {
                    res->push_back(a[i] + next - deg);
                    next = 1;
                }
                else
                {
                    h = a[i] + next;
                    res->push_back(h);
                    next = 0;
                }
            }
        }
        if (next != 0)
        {
            res->push_back(next);
        }
        return *res;
    }
    
    big_int operator - (const big_int& a) const
    {
        big_int* res = new big_int;
        int next = 0;
        int h;
        big_int this_copy(*this);
        big_int a_copy(a);
        res->set_sign(0);
        if (this->cmp(a) < 0)
        {
            this_copy = a;
            a_copy = *this;
            res->set_sign(1);
        }
        for (int i = 0; i < a_copy.size(); i++)
        {
            if (this_copy[i] - a_copy[i] - next < 0)
            {
                res->push_back(this_copy[i] - next + deg - a_copy[i]);
                next = 1;
            }
            else
            {
                h = this_copy[i] - next - a_copy[i];
                res->push_back(h);
                next = 0;
            }
        }
        for (int i = a_copy.size(); i < this_copy.size(); i++)
        {
            if (this_copy[i] - next < 0)
            {
                res->push_back(this_copy[i] - next + deg);
                next = 1;
            }
            else
            {
                res->push_back(this_copy[i] - next);
                next = 0;
            }
        }
        for (int i = res->size() - 1; (i > 0) && ((*res)[i] == 0); i--)
        {
            res->pop_back();
        }
        if ((*res)[res->size() - 1] == 0)
        {
            res->set_sign(0);
        }
        return *res;
    }
    
    big_int operator + (const int a) const
    {
        big_int _a(a);
        return (*this + _a);
    }
    
    big_int operator * (const int& _a) const
    {
        big_int* result = new big_int;
        int next = 0;
        int a;
        if (_a < 0)
        {
            if (this->get_sign() == 0)
            {
                result->set_sign(1);
            }
            else
            {
                result->set_sign(0);
            }
            a = -1 * _a;
        }
        else
        {
            result->set_sign(this->get_sign());
            a = _a;
        }
        for (int i = 0; i < this->size(); i++)
        {
            if ((*this)[i] * a + next >= deg)
            {
                result->push_back(((*this)[i] * a + next) % deg);
                next = ((*this)[i] * a + next) / deg;
            }
            else
            {
                result->push_back((*this)[i] * a + next);
                next = 0;
            }
        }
        if (next != 0)
        {
            result->push_back(next);
        }
        for (int i = result->size() - 1; i >= 0; i--)
        {
            if ((*result)[i] == 0)
            {
                result->pop_back();
            }
            else
            {
                break;
            }
        }
        return *result;
    }
    
    big_int operator * (const big_int& a) const
    {
        big_int* res = new big_int;
        int col_zero = 0;
        big_int plus_value;
        if (this->get_sign() == a.get_sign())
        {
            res->set_sign(0);
        }
        else
        {
            res->set_sign(1);
        }
        for (int i = 0; i < a.size(); i++)
        {
            plus_value = *this * a[i];
            for (int i = 1; i <= col_zero; i++)
            {
                plus_value = plus_value * 10;
            }
            *res = *res + plus_value;;
            ++col_zero;
        }
        return *res;
    }
    
    big_int operator / (const int& _a) const
    {
        if (_a < deg)
        {
            big_int* result = new big_int;
            int operation_val = (*this)[this->size() - 1];
            int a;
            if (_a < 0)
            {
                if (this->get_sign() == 0)
                {
                    result->set_sign(1);
                }
                else
                {
                    result->set_sign(0);
                }
                a = -1 * _a;
            }
            else
            {
                result->set_sign(this->get_sign());
                a = _a;
            }
            for (int i = this->size() - 1; i >= 0; i--)
            {
                if (operation_val < a)
                {
                    if (i != 0)
                    {
                        operation_val = operation_val * deg + (*this)[i - 1];
                    }
                    result->push_back(0);
                }
                else
                {
                    result->push_back(operation_val / a);
                    operation_val = ((operation_val - (operation_val / a) * a)% deg) * deg + (*this)[i - 1];
                }
            }
            result->reverse();
            for (int i = result->size() - 1; i > 0; i--)
            {
                if ((*result)[i] == 0)
                {
                    result->pop_back();
                }
                else
                {
                    break;
                }
            }
            return *result;
        }
        else
        {
            big_int a(_a);
            return *this / a;
        }
    }
    
    big_int operator % (const int& _a) const
    {
        if (_a < deg)
        {
            return *this - (*this / _a) * _a;
        }
        else
        {
            big_int a(_a);
            return *this - a * (*this / a);
        }
    }
    
    big_int operator / (const big_int& a) const
    {
        big_int this_copy;
        big_int* res = new big_int;
        if (this->cmp(a) < 0)
        {
            res->set_sign(0);
            res->push_back(0);
            return *res;
        }
        if (this->cmp(a) == 0)
        {
            res->set_sign(0);
            res->push_back(1);
            return *res;
        }
        if (this->cmp(a) > 0)
        {
            for (int i = this->size() - a.size(); i < this->size(); i++)
            {
                this_copy.push_back((*this)[i]);
            }
            for (int i = this->size() - a.size(); i >= 0; i--)
            {
                if (this_copy.cmp(a) < 0)
                {
                    this_copy.push_beg((*this)[i - 1]);
                    res->push_back(0);
                }
                else
                {
                    int j;
                    for (j = deg - 1; j >= 0; j--)
                    {
                        if (this_copy.cmp(a * j) >= 0)
                        {
                            break;
                        }
                    }
                    res->push_back(j);
                    this_copy = this_copy - a * j;
                    this_copy.push_beg((*this)[i - 1]);
                }
            }
            res->reverse();
            for (int i = res->size() - 1; i > 0; i--)
            {
                if ((*res)[i] == 0)
                {
                    res->pop_back();
                }
                else
                {
                    break;
                }
            }
            return *res;
        }
        return *res;
    }
    
    big_int operator % (const big_int& a) const
    {
        return *this - a * (*this / a);
    }
    
    int cmp(const big_int & a) const
    {
        if (this->size() == a.size())
        {
            for (int i = this->size() - 1; i >= 0; i--)
            {
                if ((*this)[i] < a[i])
                {
                    return -1;
                }
                if ((*this)[i] > a[i])
                {
                    return 1;
                }
            }
            return 0;
        }
        else
        {
            return this->size() - a.size();
        }
        return 0;
    }
    
    int size() const
    {
        return (int) body.size();
    }
    
    void push_back(int a)
    {
        this->body.push_back(a);
    }
    
    void pop_back()
    {
        this->body.pop_back();
    }
    
    void clear()
    {
        body.clear();
    }
    
    void set_sign(int a)
    {
        sign = a;
    }
    
    int get_sign() const
    {
        return sign;
    }
    
    void reverse()
    {
        int swap_val;
        for (int i = 0; i < this->size() / 2; i++)
        {
            swap_val = body[i];
            body[i] = body[this->size() - i - 1];
            body[this->size() - i - 1] = swap_val;
        }
    }
    
    void push_beg(int a)
    {
        this->push_back(0);
        for (int j = this->size() - 1; j >= 1; j--)
        {
            body[j] = body[j - 1];
        }
        body[0] = a;
    }
private:
    std::vector <int> body;
    short sign; // 1 - минус, 0 - плюс
};

std::ostream& operator << (std::ostream& os, const big_int& a) // для deg кратных 10
{
    int ten;
    if (a.get_sign() == 1)
    {
        os << "-";
    }
    os << a[a.size() - 1];
    for (int i = a.size() - 2; i >= 0; i--)
    {
        ten = deg / 10;
        while ((a[i] / ten == 0) && (ten > 1))
        {
            os << 0;
            ten /= 10;
        }
        os << a[i];
    }
    return os;
}

std::istream& operator >> (std::istream& is, big_int& a) // для deg кратных 10
{
    a.clear();
    char c;
    is.read(&c, 1);
    std::vector<int> read;
    if (c == '-')
    {
        a.set_sign(1);
        is.read(&c, 1);
    }
    else
    {
        a.set_sign(0);
    }
    while ((c >= '0') && (c <= '9'))
    {
        read.push_back(c - '0');
        is.read(&c, 1);
    }
    int ten_deg = 1;
    int max_ten_deg = deg;
    if (read.size() < log10(deg))
    {
        max_ten_deg = pow(10, read.size());
    }
    int res_val = 0;
    for (int i = (int) read.size() - 1; i >= 0; i--)
    {
        res_val += ten_deg * read[i];
        ten_deg *= 10;
        if (ten_deg == max_ten_deg)
        {
            a.push_back(res_val);
            ten_deg = 1;
            if (i <= log10(deg))
            {
                max_ten_deg = pow(10, i);
            }
            else
            {
                max_ten_deg = deg;
            }
            res_val = 0;
        }
    }
    return is;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    big_int a, b;
    int c;
    std::cin >> a >> b;
    std::cout << a + b;
    /*
    std::ifstream fin;
    fin.open("INPUT.TXT");
    std::ofstream fout;
    fout.open("OUTPUT.TXT");
    big_int a, b;
    fin >> a >> b ;
    fout << *(a - b);
    /*if (a.cmp(b) >= 0)
    {
        if (a.cmp(c) >= 0)
        {
            fout << a;
        }
        else
        {
            fout << c;
        }
    }
    else
    {
        if (a.cmp(c) >= 0)
        {
            fout << b;
        }
        else
        {
            if (b.cmp(c) >= 0)
            {
                fout << b;
            }
            else
            {
                fout << c;
            }
        }
    }
    //*/
    /*
    big_int a, b, c;
    std::cin >> a >> b;
    std::cout << a - b;
    /*if (a.cmp(b) >= 0)
    {
        if (a.cmp(c) >= 0)
        {
            std::cout << a;
        }
        else
        {
            std::cout << c;
        }
    }
    else
    {
        if (a.cmp(c) >= 0)
        {
            std::cout << b;
        }
        else
        {
            if (b.cmp(c) >= 0)
            {
                std::cout << b;
            }
            else
            {
                std::cout << c;
            }
        }
    }
    //*/
    return 0;
}
