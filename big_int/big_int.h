//
//  big_int.h
//  big_int
//
//  Created by Евгения Тверитинова on 20.04.15.
//  Copyright (c) 2015 Евгения Тверитинова. All rights reserved.
//

#ifndef __big_int__big_int__
#define __big_int__big_int__

#include <stdio.h>
#include <vector>
#include <iostream>

class big_int
{
public:
    big_int(); //
    big_int(int); //
    big_int(const big_int& a); // 
    
    big_int operator + (big_int& a) const; //
    big_int operator - (big_int& a) const;
    big_int operator * (big_int& a) const;
    big_int operator / (big_int& a) const;
    big_int operator % (big_int& a) const;
    big_int operator + (int a) const; // 
    big_int operator - (int& a) const;
    big_int operator * (int& a) const;
    big_int operator / (int& a) const;
    big_int operator % (int& a) const;
    int operator [] (int& a) const; //
    
    friend std::ostream& operator << (std::ostream& os, const big_int & a); //
    friend std::istream& operator << (std::istream& is, big_int & a);
    
    int size() const; //
    void push_back(int a); // 
private:
    std::vector<int> body;
};

#endif /* defined(__big_int__big_int__) */
