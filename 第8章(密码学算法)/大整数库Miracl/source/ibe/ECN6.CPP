/*
 *    MIRACL  C++ Implementation file ecn6.cpp
 *
 *    AUTHOR  : M. Scott
 *  
 *    PURPOSE : Implementation of class ECn6  (Elliptic curves over n^6)
 *
 * WARNING: This class has been cobbled together for a specific use with
 * the MIRACL library. It is not complete, and may not work in other 
 * applications
 *
 *    Copyright (c) 2002 Shamus Software Ltd.
 */

#include "ecn6.h"

void ECn6::get(ZZn6& a,ZZn6& b)
{a=x;b=y;}

void ECn6::get(ZZn6& a)
{a=x;}

BOOL ECn6::set(const ZZn6& xx,const ZZn6& yy)
{ if (yy*yy != xx*xx*xx+getA()*xx+getB()) return FALSE;
  x=xx;
  y=yy;
  marker=MR_EPOINT_GENERAL;
  return TRUE;
}

BOOL ECn6::set(const ZZn6& xx)
{ 
 ZZn6 w=xx*xx*xx+getA()*xx+getB();
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

ECn6 operator-(const ECn6& a) 
{ECn6 w; 
 if (a.marker!=MR_EPOINT_INFINITY) 
   {w.x=a.x; w.y=-a.y; w.marker=a.marker;} 
 return w; 
}  

ECn6& ECn6::operator*=(const Big& k)
{
    int i,j,n,nb,nbs,nzs;
    ECn6 p2,pt,t[11];
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

ECn6 operator*(const Big& r,const ECn6& P)
{
    ECn6 T=P;
    T*=r;
    return T;
}

ostream& operator<<(ostream& s,ECn6& b)
{
    ZZn6 x,y;
    if (b.iszero())
        s << "(Infinity)";
    else
    {
        b.get(x,y);
        s << "(" << x << "," << y << ")";
    }
    return s;
}

ECn6 operator+(const ECn6& a,const ECn6& b)
{ECn6 c=a; c+=b; return c;}

ECn6 operator-(const ECn6& a,const ECn6& b)
{ECn6 c=a; c-=b; return c;}

ECn6& ECn6::operator-=(const ECn6& z)
{ECn6 t=(-z); *this+=t; return *this; }

ECn6& ECn6::operator+=(const ECn6& z)
{
    ZZn6 lam;
    add(z,lam);
    return *this;
}


BOOL ECn6::add(const ECn6& z,ZZn6& lam)
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
        ZZn6 t=y;  t-=z.y;
        ZZn6 t2=x; t2-=z.x;    
        lam=t; lam/=t2;

        x+=z.x; t=lam; t*=t; t-=x; x=t;   
        y=z.x; y-=x; y*=lam; y-=z.y;   

    }
    else
    {
        if (y!=z.y || y.iszero())
        {
            clear();
            lam=(ZZn6)1;       // any non-zero value
            return TRUE;     
        }
        ZZn6 t=x;
        ZZn6 t2=x;

     //   lam=(3*(x*x)+getA())/(y+y);

        lam=x;
        lam*=lam;
        lam*=3;
        lam+=getA();
        lam/=(y+y);    
        t2+=x;
        x=lam;
        x*=x;
        x-=t2;
         
        t-=x;
        t*=lam;
        t-=y;
        y=t;            
    }

    marker=MR_EPOINT_GENERAL;    
    return TRUE;
}

