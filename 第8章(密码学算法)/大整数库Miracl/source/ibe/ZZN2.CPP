/*
 *    MIRACL  C++ Implementation file zzn2.cpp
 *
 *    AUTHOR  : M. Scott
 *  
 *    PURPOSE : Implementation of class ZZn2  (Arithmetic over n^2)
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

#include "zzn2.h"

void ZZn2::get(Big& x,Big& y)  
{x=Big(a); y=Big(b);} 

void ZZn2::get(Big& x) 
{x=Big(a); }

ZZn2& ZZn2::operator*=(const ZZn2& x)
{ // optimized to reduce constructor/destructor calls
 if (&x==this)
 { // a = (a+b)(a-b), b=2ab  (2 muls)
   ZZn t=a;  t+=b;
   ZZn t2=a; t2-=b;
   t*=t2;
   a+=a;
   b*=a;
   a=t;
 }
 else
 { // a=a*x.a-b*x.b), b=(a+b)(x.a+x.b) - a*x.a - b*x.b  (3 muls)
   ZZn t=a; t*=x.a;
   ZZn t2=b; t2*=x.b;
   ZZn t3=x.a; t3+=x.b;
   b+=a; b*=t3; b-=t; b-=t2;
   t-=t2; a=t;  
 }
 return *this;
}

ZZn2& ZZn2::operator/=(const ZZn2& x)
{
 ZZn t=x.a;
 ZZn t2=x.b;
 ZZn t3=a; 
 ZZn i=(ZZn)1;

 t*=t; t2*=t2;           // (2 sqrs + 5 muls + 1 Inverse)
 t+=t2;
 i/=t;

 t=a;   t*=x.a;   // t= a*x.a
 t2=b;  t2*=x.b;  // t2=b*x.b
 a=t;   a+=t2;   a*=i;

 b+=t3; t3=x.a;  t3-=x.b; b*=t3;
 b-=t;  b+=t2;   b*=i;

 return *this;
}

ZZn2 operator+(const ZZn2& x,const ZZn2& y) 
{ZZn2 w=x; w.a+=y.a; w.b+=y.b; return w; } 

ZZn2 operator+(const ZZn2& x,const ZZn& y) 
{ZZn2 w=x; w.a+=y; return w; } 

ZZn2 operator-(const ZZn2& x,const ZZn2& y) 
{ZZn2 w=x; w.a-=y.a; w.b-=y.b; return w; } 

ZZn2 operator-(const ZZn2& x,const ZZn& y) 
{ZZn2 w=x; w.a-=y; return w; } 

ZZn2 operator-(const ZZn2& x) 
{ZZn2 w; w.a=-x.a; w.b=-x.b; return w; } 

ZZn2 operator*(const ZZn2& x,const ZZn2& y)
{ZZn2 w=x;  w*=y;  return w;}

ZZn2 operator*(const ZZn2& x,const ZZn& y)
{ZZn2 w; w.a=x.a*y; w.b=x.b*y; return w;}

ZZn2 operator*(const ZZn& y,const ZZn2& x)
{ZZn2 w; w.a=x.a*y; w.b=x.b*y; return w;}

ZZn2 operator*(const ZZn2& x,int y)
{ZZn2 w; w.a=x.a*y; w.b=x.b*y; return w;}

ZZn2 operator*(int y,const ZZn2& x)
{ZZn2 w; w.a=x.a*y; w.b=x.b*y; return w;}

ZZn2 operator/(const ZZn2& x,const ZZn2& y)
{ZZn2 w=x; w/=y; return w;}

ZZn2 operator/(const ZZn2& x,const ZZn& y)
{ZZn2 w; ZZn i=(ZZn)1/y; w.a=x.a*i; w.b=x.b*i; return w;}

ZZn2 randn2(void)
{ZZn2 w; w.a=randn(); w.b=randn(); return w;}

ZZn2 sqrt(const ZZn2& x)
{
    ZZn2 w;
    ZZn a,s;

    if (x.iszero()) return w; 
    if (x.a.iszero())
    {
        s=sqrt(x.b/2);
        if (s.iszero()) 
        {
            s=sqrt(-x.b/2);
            w.a=-s; w.b=s;
            return w;
        }
        w.a=w.b=s;
        return w;
    }
    if (x.b.iszero())
    {
        s=sqrt(x.a);
        if (s.iszero())
        {
            s=sqrt(-x.a);
            w.a=0; w.b=s;
            return w;
        }
        w.a=s; w.b=0;
    }

    s=sqrt(x.a*x.a+x.b*x.b);
    if (s.iszero()) return w;
    a=sqrt((x.a+s)/2);
    if (a.iszero()) 
    {
        a=sqrt((x.a-s)/2);
        if (a.iszero()) return w;
    }
    w.a=a;
    w.b=x.b/(2*a);
         
    return w;
}

ZZn2 conj(const ZZn2& x)
{
    ZZn2 u=x;
    u.conj();
    return u;
}

ZZn2 pow(const ZZn2& x,const Big& k)
{
    int i,j,nb,n,nbw,nzs;
    ZZn2 u,u2,t[16];
    if (k==0) return (ZZn2)1;
    u=x;
    if (k==1) return u;
//
// Prepare table for windowing
//
    u2=(u*u);
    t[0]=u;
   
    for (i=1;i<16;i++)
        t[i]=u2*t[i-1];

// Left to right method - with windows

    nb=bits(k);
    if (nb>1) for (i=nb-2;i>=0;)
    {
        n=window(k,i,&nbw,&nzs);
        for (j=0;j<nbw;j++) u*=u;
        if (n>0) u*=t[n/2];
        i-=nbw;
        if (nzs)
        {
            for (j=0;j<nzs;j++) u*=u;
            i-=nzs;
        }
    }
    return u;
}

#ifndef MR_NO_STANDARD_IO

ostream& operator<<(ostream& s,ZZn2& b)
{
    Big x,y;
    b.get(x,y);
    s << "[" << x << "," << y << "]";
    return s;    
}

#endif

