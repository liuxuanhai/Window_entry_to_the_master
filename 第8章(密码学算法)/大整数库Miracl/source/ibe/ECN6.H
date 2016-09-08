/*
 *    MIRACL  C++ Header file ecn6.h
 *
 *    AUTHOR  : M. Scott
 *  
 *    PURPOSE : Definition of class ECn6 (Arithmetic on an Elliptic Curve,
 *               mod n^6)
 *
 *    NOTE    : Must be used in conjunction with monty.cpp, big.cpp and 
 *              zzn6.cpp
 *
 * WARNING: This class has been cobbled together for a specific use with
 * the MIRACL library. It is not complete, and may not work in other 
 * applications
 *
 *    Copyright (c) 2002 Shamus Software Ltd.
 */

#ifndef ECN6_H
#define ECN6_H

#include "zzn6.h"

class ECn6
{
    ZZn6 x,y;
    int marker;
public:
    ECn6()     {marker=MR_EPOINT_INFINITY;}
    ECn6(const ECn6& b) {x=b.x; y=b.y; marker=b.marker; }

    ECn6& operator=(const ECn6& b) 
        {x=b.x; y=b.y; marker=b.marker; return *this; }
    
    BOOL add(const ECn6&,ZZn6&);

    ECn6& operator+=(const ECn6&); 
    ECn6& operator-=(const ECn6&); 
    ECn6& operator*=(const Big&); 
   
    void clear() {x=y=0; marker=MR_EPOINT_INFINITY;}
    BOOL iszero() {if (marker==MR_EPOINT_INFINITY) return TRUE; return FALSE;}

    void get(ZZn6&,ZZn6&);
    void get(ZZn6&);
    BOOL set(const ZZn6&,const ZZn6&); // set on the curve - returns FALSE if no such point
    BOOL set(const ZZn6&);      // sets x coordinate on curve, and finds y coordinate
    
    friend ECn6 operator-(const ECn6&);
    friend ECn6 operator+(const ECn6&,const ECn6&);
    friend ECn6 operator-(const ECn6&,const ECn6&);

    friend BOOL operator==(const ECn6& a,const ECn6 &b) 
        {return (a.x==b.x && a.y==b.y && a.marker==b.marker); }
    friend BOOL operator!=(const ECn6& a,const ECn6 &b) 
        {return (a.x!=b.x || a.y!=b.y || a.marker!=b.marker); }

    friend ECn6 operator*(const Big &,const ECn6&);
    friend ostream& operator<<(ostream&,ECn6&);


    ~ECn6() {}
};

#endif

