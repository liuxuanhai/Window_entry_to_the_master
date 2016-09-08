#include "comflash.h"

BOOL Complex::iszero() const
{ if (x.iszero() && y.iszero()) return TRUE; return FALSE; }

Complex& Complex::operator=(const Complex& a)
{x=a.x; y=a.y; return *this;}

Complex& Complex::operator+=(const Complex& a)
{x+=a.x; y+=a.y; return *this;}

Complex& Complex::operator-=(const Complex& a)
{x-=a.x; y-=a.y; return *this;}

Complex& Complex::operator*=(const Complex& a)
{
    Flash t;
    if (this==&a)
    { // squaring - a bit faster
        t=x*x-y*y;
        y*=x;
        y*=2;
        x=t;
        return *this;    
    }
    t=x*a.x-y*a.y; 
    y=y*a.x+x*a.y; x=t; 
    return *this;
}

Complex& Complex::operator/=(const Complex& a)
{Flash t,d=a.x*a.x+a.y*a.y;
 t=(x*a.x+y*a.y)/d; y=(y*a.x-x*a.y)/d; x=t; return *this;}

Flash real(const Complex &a) {Flash f; f=a.x; return f;}
Flash imaginary(const Complex &a) {Flash f; f=a.y; return f;}

Complex recip(const Complex &a)
{ 
  Complex c=a;
  Flash d=c.x*c.x+c.y*c.y;
  c.x/=d;
  c.y/=d;
  c.y=-c.y;
  return c;
}

Complex operator-(const Complex& a)
{Complex c; c.x=-a.x; c.y=-a.y; return c;}

BOOL operator==(const Complex& a,const Complex& b)
{ if (a.x==b.x && a.y==b.y) return TRUE; else return FALSE;}

BOOL operator!=(const Complex& a,const Complex& b)
{ if (a.x!=b.x || a.y!=b.y) return TRUE; else return FALSE;}

Complex operator+(const Complex &a,const Complex &b)
{Complex c=a; c+=b; return c;}

Complex operator-(const Complex &a,const Complex &b)
{Complex c=a; c-=b; return c;}

Complex operator*(const Complex &a,const Complex &b)
{Complex c=a; c*=b; return c;}

Complex operator/(const Complex &a,const Complex &b)
{Complex c=a; c/=b; return c; }

Complex exp(const Complex& a)
{ // use half-angle formulae
    Complex c; 
    Flash t2p1,t2,t,d;

    d=exp(a.x);
    t=a.y;
    t/=2;
    t=tan(t);     // t=tan(theta/2)
    t2=t; t2*=t2;
    t2p1=1+t2;
    d/=t2p1;
    c.x=d*(1-t2); // cos(theta)=(1-t^2)/1+t^2)
    t*=2;
    c.y=d*t;      // sin(theta)=(2t/(1+t^2)

    return c;
}

Complex pow(const Complex& a,int b)
{Complex w=1;
 Complex c=a; 

 if (b==0) return w;
 if (b==1) return c;
 forever
 {
    if (b%2!=0) w*=c;
    b/=2;
    if (b==0) break;
    c*=c;
 }
 return w; 
}

ostream& operator<<(ostream& s,const Complex&a)
{
    s << "(" << a.x << "," << a.y << ")";
    return s;
}

