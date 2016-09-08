/*
 *    MIRACL  C++ Implementation file zzn6.cpp
 *
 *    AUTHOR  : M. Scott
 *  
 *    PURPOSE : Implementation of class ZZn6  (Arithmetic over n^6)
 *
 * WARNING: This class has been cobbled together for a specific use with
 * the MIRACL library. It is not complete, and may not work in other 
 * applications
 *
 *    NOTE: - The irreducible polynomial is assumed to be of the form 
 *            x^6+x^k+n
 *
 * The file zzn6.dat is created by the irred utility
 *
 *    Copyright (c) 2002 Shamus Software Ltd.
 */

#include "zzn6.h"
#include <fstream>
using std::ifstream;

// Field Globals

int N,K,S;         // irreducible f(x) = x^6+x^K+N, 2^S|p^6-1
ZZn6 X[6],E;       // X[i]=x^(i*p) mod f(x)
Big ST[6];         // Square rooting exponent

void init_zzn6(Big p)
{
    int i,j;
    Big xx[6],t;
    ZZn6 d;

    modulo(p);
    ifstream fd("zzn6.dat");
    get_mip()->IOBASE=10;

    fd >> N;
    fd >> K;    
    for (i=0;i<6;i++) fd >> xx[i];
    X[1].set(xx);
    for (j=2;j<6;j++)
    {
        for (i=0;i<6;i++) fd >> xx[i];
        X[j].set(xx);
    }
    fd >> S;
    for (i=0;i<6;i++) fd >> ST[i];

//  Calculate 2^s root of unity E
    do {                          // HAC 3.34 Step 2
        for (i=0;i<6;i++) xx[i]=rand(p);
        E.set(xx);                // e can be pre-computed off-line
        E=pow(E,ST);              // e is required 2^s root of unity
        d=E; for (i=1;i<S;i++) d*=d;    
    } while (d.isunity());

    for (i=0;i<6;i++) fd >> ST[i];
}

ZZn6& ZZn6::powq()
{
    *this=a[5]*X[5]+a[4]*X[4]+a[3]*X[3]+a[2]*X[2]+a[1]*X[1]+a[0];
    return *this;
}

int degree(const ZZn6& x)
{
    if (x.a[5]!=0) return 5;
    if (x.a[4]!=0) return 4;
    if (x.a[3]!=0) return 3;
    if (x.a[2]!=0) return 2;
    if (x.a[1]!=0) return 1;
    if (x.a[0]!=0) return 0;
    return -1;
}

void ZZn6::get(Big* x)  
{for (int i=0;i<6;i++) x[i]=(Big)a[i]; } 

void ZZn6::get(Big& x) 
{x=Big(a[0]); }

ZZn6& ZZn6::operator*=(const ZZn6& x)
{ 
    int n=N;
    int g=K;

    if (&x==this)
    {  // Karatsuba squaring
        if (a[5].iszero() && a[3].iszero() && a[1].iszero())
        {
            if (a[4].iszero() && a[2].iszero())
            {
                a[0]*=a[0];
                return *this;
            }

            ZZn r[6];

            r[0] = a[0]*a[0];
            r[2] = a[2]*a[2];
            r[4] = a[4]*a[4];

            r[1] = (a[0] + a[2]); r[1] *= r[1]; r[1] -= (r[0] + r[2]);
            r[3] = (a[2] + a[4]); r[3] *= r[3]; r[3] -= (r[2] + r[4]);
            r[5] = (a[0] + a[4]); r[5] *= r[5]; r[5] -= (r[0] + r[4]);
            r[2] += r[5];

            r[2] -= r[4]; r[1] -= n*r[4];
            r[1] -= r[3]; r[0] -= n*r[3];

            a[0] = r[0]; a[1] = 0;
            a[2] = r[1]; a[3] = 0;
            a[4] = r[2]; a[5] = 0;
            return *this;
        }

        ZZn r[18];

        r[ 0] = a[0]*a[0];
        r[ 2] = a[1]*a[1];
        r[ 4] = a[2]*a[2];
        r[ 6] = a[3]*a[3];
        r[ 8] = a[4]*a[4];
        r[10] = a[5]*a[5];

        r[11] = (a[0] + a[2]); r[11] *= r[11]; r[11] -= (r[ 0] + r[ 4]);
        r[12] = (a[0] + a[4]); r[12] *= r[12]; r[12] -= (r[ 0] + r[ 8]);
        r[13] = (a[1] + a[3]); r[13] *= r[13]; r[13] -= (r[ 2] + r[ 6]);
        r[14] = (a[1] + a[5]); r[14] *= r[14]; r[14] -= (r[ 2] + r[10]);
        r[15] = (a[2] + a[3]); r[15] *= r[15]; r[15] -= (r[ 4] + r[ 6]);
        r[16] = (a[2] + a[4]); r[16] *= r[16]; r[16] -= (r[ 4] + r[ 8]);
        r[17] = (a[3] + a[5]); r[17] *= r[17]; r[17] -= (r[ 6] + r[10]);

        r[ 1] = (a[0] + a[1]); r[ 1] *= r[ 1]; r[ 1] -= (r[ 0] + r[ 2]);
        r[ 9] = (a[4] + a[5]); r[ 9] *= r[ 9]; r[ 9] -= (r[ 8] + r[10]);

        r[ 3] = (a[0] + a[1] + a[2] + a[3]); r[ 3] *= r[ 3];
        r[ 3] -=(r[0] + r[2] + r[4] + r[6] + r[ 1] + r[11] + r[13] + r[15]);

        r[ 5] = (a[0] + a[1] + a[4] + a[5]); r[ 5] *= r[ 5];
        r[ 5] -=(r[0] + r[2] + r[8] + r[10] + r[1] + r[12] + r[14] + r[9]);

        r[ 7] = (a[2] + a[3] + a[4] + a[5]); r[ 7] *= r[ 7];
        r[ 7] -=(r[4] + r[6] + r[8] + r[10] + r[15] + r[16] + r[17] + r[9]);
        r[ 8] += r[17];
        r[ 6] += r[14] + r[16];
        r[ 5] += r[15];
        r[ 4] += r[12] + r[13];
        r[ 2] += r[11];

        r[4+g] -= r[10]; r[4] -= n*r[10];
        r[3+g] -= r[ 9]; r[3] -= n*r[ 9];
        r[2+g] -= r[ 8]; r[2] -= n*r[ 8];
        r[1+g] -= r[ 7]; r[1] -= n*r[ 7];
        r[  g] -= r[ 6]; r[0] -= n*r[ 6];

        a[0] = r[0]; a[1] = r[1];
        a[2] = r[2]; a[3] = r[3];
        a[4] = r[4]; a[5] = r[5];

     }
     else
     { // Karatsuba multiplication

        if (a[5].iszero() && x.a[5].iszero()
         && a[3].iszero() && x.a[3].iszero()
         && a[1].iszero() && x.a[1].iszero())
        {
            if (a[4].iszero() && x.a[4].iszero()
             && a[2].iszero() && x.a[2].iszero())
            {
                a[0]*=x.a[0];
                return *this;
            }   

            ZZn r[6];

            r[0] = a[0]*x.a[0];
            r[2] = a[2]*x.a[2];
            r[4] = a[4]*x.a[4];

            r[1] = (a[0] + a[2])*(x.a[0] + x.a[2]) - (r[0] + r[2]);
            r[3] = (a[2] + a[4])*(x.a[2] + x.a[4]) - (r[2] + r[4]);
            r[5] = (a[0] + a[4])*(x.a[0] + x.a[4]) - (r[0] + r[4]);

            r[2] += r[5];

            r[2] -= r[4]; r[1] -= n*r[4];
            r[1] -= r[3]; r[0] -= n*r[3];

            a[0] = r[0]; a[1] = 0;
            a[2] = r[1]; a[3] = 0;
            a[4] = r[2]; a[5] = 0;
            return *this;
        }

        ZZn r[18];

        r[ 0] = a[0]*x.a[0];
        r[ 2] = a[1]*x.a[1];
        r[ 4] = a[2]*x.a[2];
        r[ 6] = a[3]*x.a[3];
        r[ 8] = a[4]*x.a[4];
        r[10] = a[5]*x.a[5];

        r[11] = (a[0] + a[2])*(x.a[0] + x.a[2]) - (r[ 0] + r[ 4]);
        r[12] = (a[0] + a[4])*(x.a[0] + x.a[4]) - (r[ 0] + r[ 8]);
        r[13] = (a[1] + a[3])*(x.a[1] + x.a[3]) - (r[ 2] + r[ 6]);
        r[14] = (a[1] + a[5])*(x.a[1] + x.a[5]) - (r[ 2] + r[10]);
        r[15] = (a[2] + a[3])*(x.a[2] + x.a[3]) - (r[ 4] + r[ 6]);
        r[16] = (a[2] + a[4])*(x.a[2] + x.a[4]) - (r[ 4] + r[ 8]);
        r[17] = (a[3] + a[5])*(x.a[3] + x.a[5]) - (r[ 6] + r[10]);

        r[ 1] = (a[0] + a[1])*(x.a[0] + x.a[1]) - (r[ 0] + r[ 2]);
        r[ 9] = (a[4] + a[5])*(x.a[4] + x.a[5]) - (r[ 8] + r[10]);
        r[ 3] = (a[0] + a[1] + a[2] + a[3])*(x.a[0] + x.a[1] + x.a[2] + x.a[3]) -
                (r[ 0] + r[ 2] + r[ 4] + r[ 6] + r[ 1] + r[11] + r[13] + r[15]);
        r[ 5] = (a[0] + a[1] + a[4] + a[5])*(x.a[0] + x.a[1] + x.a[4] + x.a[5]) -
                (r[ 0] + r[ 2] + r[ 8] + r[10] + r[ 1] + r[12] + r[14] + r[ 9]);
        r[ 7] = (a[2] + a[3] + a[4] + a[5])*(x.a[2] + x.a[3] + x.a[4] + x.a[5]) -
                (r[ 4] + r[ 6] + r[ 8] + r[10] + r[15] + r[16] + r[17] + r[ 9]);

        r[ 8] += r[17];
        r[ 6] += r[14] + r[16];
        r[ 5] += r[15];
        r[ 4] += r[12] + r[13];
        r[ 2] += r[11];

        r[4+g] -= r[10]; r[4] -= n*r[10];
        r[3+g] -= r[ 9]; r[3] -= n*r[ 9];
        r[2+g] -= r[ 8]; r[2] -= n*r[ 8];
        r[1+g] -= r[ 7]; r[1] -= n*r[ 7];
        r[  g] -= r[ 6]; r[0] -= n*r[ 6];

        a[0] = r[0]; a[1] = r[1];
        a[2] = r[2]; a[3] = r[3];
        a[4] = r[4]; a[5] = r[5];
    }

    return *this;
}

ZZn6& ZZn6::invert(void)
{
    int degF,degG,degB,degC,i,j,n,d,g;
    ZZn BB[7],FF[7],CC[7],GG[7],alpha,beta,gamma;
    ZZn *B=BB,*C=CC,*F=FF,*G=GG,*T;

    ZZn6 t,keep=*this;

    n=N;
    g=K;

    C[0]=1; F[0]=n; F[g]=1; F[6]=1;  // F=x^6+x^K+n
    degF=6; degG=degree(*this); degC=0; degB=-1;

    if (degG==0)
    {
        a[0]=(ZZn)1/a[0];
        return *this;
    }


    for (i=0;i<6;i++)
    {
        G[i]=a[i];
        a[i]=0;
    }

    while (degF!=0)
    {
        if (degF<degG)
        {
            T=F; F=G; G=T; d=degF; degF=degG; degG=d;
            T=B; B=C; C=T; d=degB; degB=degC; degC=d;
        }
        j=degF-degG;
        alpha=G[degG]*G[degG];
        beta=F[degF]*G[degG];
        gamma=G[degG]*F[degF-1]-F[degF]*G[degG-1];

        for (i=0;i<=degF;i++)
        {
            F[i]*=alpha;
            if (i>=j-1) F[i]-=gamma*G[i-j+1];
            if (i>=j)   F[i]-=beta*G[i-j];
        }


        for (i=0;i<=degB || i<=degC+j;i++)
        {
            B[i]*=alpha;
            if (i>=j-1) B[i]-=gamma*C[i-j+1];
            if (i>=j)   B[i]-=beta*C[i-j];
        }        

        while (degF>=0 && F[degF]==0) degF--;

        if (degF==degG)
        {
            alpha=F[degF];
            for (i=0;i<=degF;i++)
            {
                F[i]*=G[degF];
                F[i]-=alpha*G[i];
            }
            for (i=0;i<=6-degF;i++) 
            {
                B[i]*=G[degF];
                B[i]-=alpha*C[i];
            } 
            while (degF>=0 && F[degF]==0) degF--;
        }

        degB=5; while (degB>=0 && B[degB]==0) degB--;
    
    }

    alpha=(ZZn)1/F[0];
    for (i=0;i<=degB;i++)
        a[i]=alpha*B[i];

//    if (!(*this*keep).isunity()) cout << "inverse Failed" << endl;

    return *this;    
}

ZZn6& ZZn6::operator/=(const ZZn6& x)
{ // inversion 
 ZZn6 y=x;
 y.invert();
 *this *= y;
 return *this;
}

ZZn6 operator+(const ZZn6& x,const ZZn6& y) 
{ZZn6 w=x; for (int i=0;i<6;i++) w.a[i]+=y.a[i]; return w;} 

ZZn6 operator+(const ZZn6& x,const ZZn& y) 
{ZZn6 w=x; w.a[0]+=y; return w; } 

ZZn6 operator-(const ZZn6& x,const ZZn6& y) 
{ZZn6 w=x; for (int i=0;i<6;i++) w.a[i]-=y.a[i]; return w; } 

ZZn6 operator-(const ZZn6& x,const ZZn& y) 
{ZZn6 w=x; w.a[0]-=y; return w; } 

ZZn6 operator-(const ZZn6& x) 
{ZZn6 w; for (int i=0;i<6;i++) w.a[i]=-x.a[i]; return w; } 

ZZn6 operator*(const ZZn6& x,const ZZn6& y)
{ZZn6 w=x;  w*=y;  return w;}

ZZn6 operator*(const ZZn6& x,const ZZn& y)
{ZZn6 w=x; for (int i=0;i<6;i++) w.a[i]*=y; return w;}

ZZn6 operator*(const ZZn& y,const ZZn6& x)
{ZZn6 w=x; for (int i=0;i<6;i++) w.a[i]*=y; return w;}

ZZn6 operator*(const ZZn6& x,int y)
{ZZn6 w=x; for (int i=0;i<6;i++) w.a[i]*=y; return w;}
                                         
ZZn6 operator*(int y,const ZZn6& x)
{ZZn6 w=x; for (int i=0;i<6;i++) w.a[i]*=y; return w;}

ZZn6 operator/(const ZZn6& x,const ZZn6& y)
{ZZn6 w=x; w/=y; return w;}

ZZn6 operator/(const ZZn6& x,const ZZn& y)
{ZZn6 w=x; ZZn j=(ZZn)1/y; for (int i=0;i<6;i++) w.a[i]*=j; return w;}

ZZn6 randn6(void)
{ZZn6 w; for (int i=0;i<6;i++) w.a[i]=randn(); return w;}

#ifndef MR_NO_STANDARD_IO

ostream& operator<<(ostream& s,ZZn6& b)
{
    int i;
    Big x[6];
    b.get(x);
    s << "[";
    for (i=0;i<=4;i++)
        s << x[i] << ",";
    s << x[5] << "]";
    return s;    
}

#endif

ZZn6 sqrt(const ZZn6& x)
{ // Square Root 
    ZZn6 d,c,r,xm1;
    int i,j;
    Big xx[6];

    xm1=x;                   // HAC Step 4
    xm1.invert();

    r=pow(x,ST);             // HAC Step 5 - time consuming bit...

    c=E;
    for (j=1;j<S;j++)        // HAC Step 6
    {
        d=r*r*xm1;
        for (i=1;i<S-j;i++) d*=d;
        if (d.isminusone()) r=r*c;
        c*=c;
    }
    if (r*r!=x) r.clear();

    return r;
}

// Left to right method - with windows

ZZn6 pow(const ZZn6* t,const ZZn6& x,const Big& k)
{
    int i,j,nb,n,nbw,nzs;
    ZZn6 u=x;

    if (k==0) return (ZZn6)1;
    if (k==1) return u;

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

void precompute(const ZZn6& x,ZZn6* t)
{
    int i;
    ZZn6 u2,u=x;
    u2=(u*u);
    t[0]=u;
   
    for (i=1;i<16;i++)
        t[i]=u2*t[i-1];

}

ZZn6 pow(const ZZn6& x,const Big& k)
{
    ZZn6 u,t[16];

    if (k==0) return (ZZn6)1;
    u=x;
    if (k==1) return u;
//
// Prepare table for windowing
//
    precompute(u,t);
    return pow(t,u,k);
}

ZZn6 pow(const ZZn6& x,const Big* k)
{
    int i;
    ZZn6 t[16],r=x;

    precompute(x,t);   // only once

    r=pow(t,x,k[5]);
    for (i=4;i>=0;i--)
    {
        r.powq();
        r*=pow(t,x,k[i]);
    }

    return r;
}

