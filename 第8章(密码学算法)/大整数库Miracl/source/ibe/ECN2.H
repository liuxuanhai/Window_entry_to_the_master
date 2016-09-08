/*
 *    MIRACL  C++ Header file ecn2.h
 *
 *    AUTHOR  : M. Scott
 *  
 *    PURPOSE : Definition of class ECn2 (Arithmetic on an Elliptic Curve,
 *               mod n^2)
 *
 *    NOTE    : Must be used in conjunction with monty.cpp, big.cpp and 
 *              zzn2.cpp
 *
 * WARNING: This class has been cobbled together for a specific use with
 * the MIRACL library. It is not complete, and may not work in other 
 * applications
 *
 *    Copyright (c) 2001 Shamus Software Ltd.
 */

#ifndef ECN2_H
#define ECN2_H

#include "zzn2.h"

class ECn2
{
    ZZn2 x,y;
    int marker;
public:
    ECn2()     {marker=MR_EPOINT_INFINITY;}
    ECn2(const ECn2& b) {x=b.x; y=b.y; marker=b.marker; }

    ECn2& operator=(const ECn2& b) 
        {x=b.x; y=b.y; marker=b.marker; return *this; }
    
    BOOL add(const ECn2&,ZZn2&);

    ECn2& operator+=(const ECn2&); 
    ECn2& operator-=(const ECn2&); 
    ECn2& operator*=(const Big&); 
   
    void clear() {x=y=0; marker=MR_EPOINT_INFINITY;}
    BOOL iszero() {if (marker==MR_EPOINT_INFINITY) return TRUE; return FALSE;}

    void get(ZZn2&,ZZn2&);
    void get(ZZn2&);
    BOOL set(const ZZn2&,const ZZn2&); // set on the curve - returns FALSE if no such point
    BOOL set(const ZZn2&);      // sets x coordinate on curve, and finds y coordinate
    
    friend ECn2 operator-(const ECn2&);
    friend ECn2 operator+(const ECn2&,const ECn2&);
    friend ECn2 operator-(const ECn2&,const ECn2&);

    friend BOOL operator==(const ECn2& a,const ECn2 &b) 
        {return (a.x==b.x && a.y==b.y && a.marker==b.marker); }
    friend BOOL operator!=(const ECn2& a,const ECn2 &b) 
        {return (a.x!=b.x || a.y!=b.y || a.marker!=b.marker); }

    friend ECn2 operator*(const Big &,const ECn2&);

#ifndef MR_NO_STANDARD_IO
    friend ostream& operator<<(ostream&,ECn2&);
#endif

    ~ECn2() {}
};

#endif

