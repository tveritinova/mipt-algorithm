//
//  big_int.cpp
//  big_int
//
//  Created by Евгения Тверитинова on 20.04.15.
//  Copyright (c) 2015 Евгения Тверитинова. All rights reserved.
//

#include <stdio.h>
#include <vector>
#include <iostream>

int deg = 100;

class big_int
{
public:
    big_int() {}

    big_int(int a)
    {
        int t = 1;
        int s;
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
    }

    int operator [] (int& a) const
    {
        return body[a];
    }

    operator + (big_int& a) const
    {
        big_int res;
        int cnt = 0;
        int size;
        int next = 0;
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
                res.push_back(a[i] + (*this)[i] + next - deg);
                next = (a[i] + (*this)[i] + next) / deg;
            }
            else
            {
                res.push_back(a[i] + (*this)[i] + next);
                next = 0;
            }
        }
        res.push_back(next + a[size] + (*this)[size]);
        if (a.size() < this->size())
        {
            for (int i = size + 1; i < this->size(); i++)
            {
                res.push_back((*this)[i]);
            }
        }
        if (a.size() > this->size())
        {
            for (int i = size + 1; i < a.size(); i++)
            {
                res.push_back(a[i]);
            }
        }
        return res;
    }

    big_int operator + (int a) const
    {
        big_int _a(a);
        return *this + _a;
    }

    std::ostream& operator << (std::ostream& os, const big_int& a)
    {
        for (int i = 0; i < a.size(); i++)
        {
            os << a[i] << " ";
        }
        os << std::endl;
        return os;
    }


    int size() const
    {
        return body.size();
    }
    
    void push_back(int a)
    {
        this->body.push_back(a);
    }