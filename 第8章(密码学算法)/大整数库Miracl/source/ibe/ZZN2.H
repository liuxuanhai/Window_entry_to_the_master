/*
 *    MIRACL  C++ Header file ZZn2.h
 *
 *    AUTHOR  : M. Scott
 *
 *    NOTE:   : Must be used in conjunction with big.cpp and monty.cpp
 *
 *    PURPOSE : Definition of class ZZn2  (Arithmetic over n^2)
 *
 * WARNING: This class has been cobbled together for a specific use with
 * the MIRACL library. It is not complete, and may not work in other 
 * applications
 *
 *    Note: This code assumes that -1 is a Quadratic Non-Residue,
 *          In other words the modulus p is a prime = 3 mod 4
 *
 *    Copyright (c) 2001 Shamus Software Ltd.
 */

#ifndef ZZN2_H
#define ZZN2_H

#include <monty.h>

class ZZn2
{
    ZZn a,b;
public:
    ZZn2()   {}
    ZZn2(int w) {a=(ZZn)w; b=0;}
    ZZn2(const ZZn2& w) {a=w.a; b=w.b; }
    ZZn2(const Big &x,const Big& y) {a=(ZZn)x; b=(ZZn)y; }
    ZZn2(const Big &x)              {a=(ZZn)x; b=0; }
    
    void set(const Big &x,const Big &y) {a=(ZZn)x; b=(ZZn)y; }
    void set(const Big &x)              {a=(ZZn)x; b=0; }

    void get(Big &,Big &) ;
    void get(Big &) ;
    
    void conj() {b=-b;}
    void clear() {a=0; b=0; }
    
    BOOL iszero()  const {if (a==0 && b==0) return TRUE; return FALSE; }
    BOOL isunity() const {if (a==1 && b==0) return TRUE; return FALSE; }
    BOOL isminusone() const {if (a==(-1) && b==0) return TRUE; return FALSE; }
    ZZn2& operator=(int i) {a=i; b=0; return *this;}
    ZZn2& operator=(const ZZn& x) {a=x; b=0; return *this; }
    ZZn2& operator=(const ZZn2& x) {a=x.a; b=x.b; return *this; }
    ZZn2& operator+=(const ZZn& x) {a+=x; return *this; }
    ZZn2& operator+=(const ZZn2& x) {a+=x.a; b+=x.b; return *this; }
    ZZn2& operator-=(const ZZn& x) {a-=x; return *this; }
    ZZn2& operator-=(const ZZn2& x) {a-=x.a; b-=x.b; return *this; }
    ZZn2& operator*=(const ZZn2&); 
    ZZn2& operator*=(const ZZn& x) {a*=x; b*=x; return *this; }
    ZZn2& operator*=(int x) {a*=x; b*=x; return *this;}
    ZZn2& operator/=(const ZZn2&); 
    ZZn2& operator/=(const ZZn& x) {a/=x; b/=x; return *this; }

    friend ZZn2 operator+(const ZZn2&,const ZZn2&);
    friend ZZn2 operator+(const ZZn2&,const ZZn&);
    friend ZZn2 operator-(const ZZn2&,const ZZn2&);
    friend ZZn2 operator-(const ZZn2&,const ZZn&);
    friend ZZn2 operator-(const ZZn2&);

    friend ZZn2 operator*(const ZZn2&,const ZZn2&);
    friend ZZn2 operator*(const ZZn2&,const ZZn&);
    friend ZZn2 operator*(const ZZn&,const ZZn2&);

    friend ZZn2 operator*(int,const ZZn2&);
    friend ZZn2 operator*(const ZZn2&,int);

    friend ZZn2 operator/(const ZZn2&,const ZZn2&);
    friend ZZn2 operator/(const ZZn2&,const ZZn&);

    friend ZZn2 pow(const ZZn2&,const Big&);
    friend ZZn2 conj(const ZZn2&);

    friend ZZn2 randn2(void);        // random ZZn2
    friend ZZn2 sqrt(const ZZn2&);   // square root - 0 if none exists

    friend BOOL operator==(const ZZn2& x,const ZZn2& y)
    {if (x.a==y.a && x.b==y.b) return TRUE; else return FALSE; }

    friend BOOL operator!=(const ZZn2& x,const ZZn2& y)
    {if (x.a!=y.a || x.b!=y.b) return TRUE; else return FALSE; }

#ifndef MR_NO_STANDARD_IO
    friend ostream& operator<<(ostream&,ZZn2&);
#endif

    ~ZZn2()  {}
};

#endif

