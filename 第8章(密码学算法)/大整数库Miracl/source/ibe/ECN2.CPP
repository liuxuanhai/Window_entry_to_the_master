/*
 *    MIRACL  C++ Implementation file ecn2cpp
 *
 *    AUTHOR  : M. Scott
 *  
 *    PURPOSE : Implementation of class ECn2  (Elliptic curves over n^2)
 *
 * WARNING: This class has been cobbled together for a specific use with
 * the MIRACL library. It is not complete, and may not work in other 
 * applications
 *
 *    Copyright (c) 2001 Shamus Software Ltd.
 */


#include "ecn2.h"

void ECn2::get(ZZn2& a,ZZn2& b)
{a=x;b=y;}

void ECn2::get(ZZn2& a)
{a=x;}

BOOL ECn2::set(const ZZn2& xx,const ZZn2& yy)
{ if (yy*yy != xx*xx*xx+getA()*xx+getB()) return FALSE;
  x=xx;
  y=yy;
  marker=MR_EPOINT_GENERAL;
  return TRUE;
}

BOOL ECn2::set(const ZZn2& xx)
{ 
 ZZn2 w=xx*xx*xx+getA()*xx+getB();
 if (!w.iszero())
 {
  w=sqrt(w); 
  if (w.iszero()) return FALSE;
 }
 x=xx;
 y=w;
 marker=MR_EPOINT_GENERAL;
 return TRUE;
}

ECn2 operator-(const ECn2& a) 
{ECn2 w; 
 if (a.marker!=MR_EPOINT_INFINITY) 
   {w.x=a.x; w.y=-a.y; w.marker=a.marker;} 
 return w; 
}  

ECn2& ECn2::operator*=(const Big& k)
{
    int i,j,n,nb,nbs,nzs;
    ECn2 p2,pt,t[11];
    Big h;

    if (k==0)
    {
        clear();
        return *this;
    }
    if (k==1)
    {
        return (*this);
    }
    h=3*k;

    pt=*this;
    p2=pt+pt; 
    t[0]=pt;
    for (i=1;i<=10;i++)
        t[i]=t[i-1]+p2;

// Left to Right method

    nb=bits(h);
    for (i=nb-2;i>=1;)
    {
        n=naf_window(k,h,i,&nbs,&nzs);
        for (j=0;j<nbs;j++) pt+=pt;
        if (n>0) pt+=t[n/2];
        if (n<0) pt-=t[(-n)/2];
        i-=nbs;
        if (nzs)
        {
            for (j=0;j<nzs;j++) pt+=pt;
            i-=nzs;
        }
    }
    *this=pt;
    return *this;
}

ECn2 operator*(const Big& r,const ECn2& P)
{
    ECn2 T=P;
    T*=r;
    return T;
}

#ifndef MR_NO_STANDARD_IO

ostream& operator<<(ostream& s,ECn2& b)
{
    ZZn2 x,y;
    if (b.iszero())
        s << "(Infinity)";
    else
    {
        b.get(x,y);
        s << "(" << x << "," << y << ")";
    }
    return s;
}

#endif

ECn2 operator+(const ECn2& a,const ECn2& b)
{ECn2 c=a; c+=b; return c;}

ECn2 operator-(const ECn2& a,const ECn2& b)
{ECn2 c=a; c-=b; return c;}

ECn2& ECn2::operator-=(const ECn2& z)
{ECn2 t=(-z); *this+=t; return *this; }

ECn2& ECn2::operator+=(const ECn2& z)
{
    ZZn2 lam;
    add(z,lam);
    return *this;
}

BOOL ECn2::add(const ECn2& z,ZZn2& lam)
{
    if (marker==MR_EPOINT_INFINITY)
    {
        *this=z;
        return FALSE;
    }
    if (z.marker==MR_EPOINT_INFINITY)
    {
        return FALSE;
    }

    if (x!=z.x)
    {
        ZZn2 t=y;  t-=z.y;
        ZZn2 t2=x; t2-=z.x;                 // 2 ZZn sqrs, 5 muls, 1 Inverse
        lam=t; lam/=t2;

        x+=z.x; t=lam; t*=t; t-=x; x=t;     // 5 ZZn muls
        y=z.x; y-=x; y*=lam; y-=z.y;   

    }
    else
    {
        if (y!=z.y || y.iszero())
        {
            clear();
            lam=(ZZn2)1; 
            return TRUE;    // any non-zero value
        }
        ZZn2 t=x;
        ZZn2 t2=x;

     //   lam=(3*(x*x)+getA())/(y+y);

        lam=x;
        lam*=lam;
        lam*=3;
        lam+=getA();
        lam/=(y+y);                         // 2 sqrs, 7 muls and 1 inverse

        t2+=x;
        x=lam;
        x*=x;
        x-=t2;
         
        t-=x;
        t*=lam;
        t-=y;
        y=t;                                // 5 ZZn muls
    }

    marker=MR_EPOINT_GENERAL;    
    return TRUE;
}

