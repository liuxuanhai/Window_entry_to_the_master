/*
 * cm.cpp - Finding an elliptic curve and point of nearly prime order
 * See IEEE 1363 Annex A for documentation!
 *
 * !!! New much improved version - March 2002
 * Now tested for D up to 10^7
 *
 * Note that less precision (one-third) is needed if D is not divisible by 3
 * See Lay & Zimmer "Constructing Elliptic curves with Given Group Order over 
 * Large Finite Fields"
 *
 * Uses functions from the MIRACL multiprecision library, specifically
 * classes:-
 * Flash   - Big floating point (actually floating slash - but looks like 
 *                               floating point)
 * Complex - Big Complex flash
 * Big     - Big integer
 * ZZn     - Big integers mod an integer
 * FPoly   - Big Flash polynomial
 * Poly    - Big ZZn polynomial
 *
 * Written by Mike Scott, Dublin, Ireland. March 1998 
 *
 * Full MIRACL source is available from 
 * ftp.computing.dcu.ie/pub/crypto/miracl.zip
 *
 * MIRACL is a shareware source code product.
 * However it is free for educational and non-profit making use
 * Queries to mike@computing.dcu.ie
 * Web page http://indigo.ie/~mscott
 */

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <elliptic.h>
#include "comflash.h"
#include "fpoly.h"
#include "poly.h"

using namespace std;

miracl *mip;

FPoly T;  // Reduced class Polynomial. 
static char *s;
BOOL fout,suppress;

// F(z) Function A.13.3

Complex Fz(Complex z)
{
    Complex t;
    int sign=1;
    Complex sum=(Flash)1;

    if (z.iszero()) return sum;

    Complex zi=z;
    Complex zj=z*z;
    Complex r=z;
    Complex z3=zj*z;

    forever
    {
        if (zi.iszero() && zj.iszero()) break;   
        t=zi+zj;
        if (sign) sum-=t;
        else      sum+=t;

        r*=z3;        
        zi*=r; zj*=r; zj*=z;

        sign=1-sign;
    }

    return sum;
}

// Fj(A,B,C) function A.13.3

Complex F(int j,Big A,Big B,Big C)
{
    Complex t,theta24,theta,theta2;
    Flash sd;
    Big D=A*C-B*B;
    sd=-sqrt((Flash)D);

    t=Complex(sd*pi()/(24*A),(pi()*B)/(24*A));    
    theta24=exp(t);                                // theta^(1/24)
    theta=pow(theta24,24);                         // theta

    if (j!=2) 
        t=recip(theta24);      // -24th root
    else  
        t=theta24*theta24;     // 12th root

    theta2=theta;
    theta2*=theta2;
    if (j==0) return (t*Fz(-theta)/Fz(theta2));
    if (j==1) return (t*Fz(theta)/Fz(theta2));
    if (j==2) return (sqrt((Flash)2)*t*Fz(theta2*theta2)/Fz(theta2));
    return 0;
}

int geti(Big D)
{
    Big d=D%8;
    if (d==1 || d==2 || d==6 || d==7) return 3;
    if (d==3)
    {
       if (D%3==0) return 2;
       else        return 0;
    } 
    if (d==5) return 6;
    return 0;
}

int getk(Big D)
{
    Big d=D%8;
    if (d==1 || d==2 || d==6) return 2;
    if (d==3 || d==7) return 1;
    if (d==5) return 4;
    return 0; 
}

// A.13.3

void class_poly(Complex& lam,Flash *Fi2,Big A,Big B,Big C,Big D,BOOL conj)
{
    Big ac,l,t;
    int i,j,k,g,e,m,n,dm8,a2,c2;
    Complex cinv;

    g=1;
    if (D%3==0) g=3;

    ac=A*C;
    if (ac%2==1) 
    {
        j=0;
        l=A-C+A*A*C;
    }
    if (C%2==0)  j=1;
    if (A%2==0)  j=2;

    if (A%2==0)
    {
        t=(C*C-1)/8;
        if (t%2==0) m=1;
        else        m=-1;
    }
    else
    {
        t=(A*A-1)/8;
        if (t%2==0) m=1;
        else        m=-1;
    }
    
    dm8=D%8;
    i=geti(D);
    k=getk(D);
    switch (dm8)
    {
    case 1: 
    case 2:    n=m;
               if (C%2==0) l=A+2*C-A*C*C;
               if (A%2==0) l=A-C-A*C*C;
               break;
    case 3:    if (ac%2==1) n=1;
               else         n=-m;
               if (C%2==0) l=A+2*C-A*C*C;
               if (A%2==0) l=A-C+5*A*C*C;
               break;
    case 5:    n=1;
               if (C%2==0) l=A-C+A*A*C;
               if (A%2==0) l=A-C-A*C*C;
               break;
    case 6:    n=m;
               if (C%2==0) l=A+2*C-A*C*C;
               if (A%2==0) l=A-C-A*C*C;
               break;
    case 7:    if (ac%2==0) n=1;
               else         n=m;
               if (C%2==0) l=A+2*C-A*C*C;
               if (A%2==0) l=A-C-A*C*C;
               break;
               
    default: break;
    }

    e=(k*B*l)%48;
    if (e<0) e+=48;
    cinv=pow(lam,e);
    cinv*=(n*Fi2[i]);
    cinv=pow(cinv*pow(F(j,A,B,C),k),g);

 // multiply polynomial by new term(s)
    FPoly F;
    if (conj)
    { // conjugate pair
      // t^2-2a+(a^2+b^2) , where cinv=a+ib
        F.addterm((Flash)1,2);
        F.addterm(-2*real(cinv),1);
        F.addterm(real(cinv)*real(cinv)+imaginary(cinv)*imaginary(cinv),0);
    }
    else 
    { // t-cinv
        F.addterm((Flash)1,1);
        F.addterm(-real(cinv),0);
    }
    T=T*F;    // accumulate Polynomial
}

// A.13.2

int groups(Complex& lam,Flash *Fi2,Big D,BOOL doit,Flash& sigma)
{
    Big s,t,A,C,B,lim;
    int cn=0;
    s=sqrt(D/3);
    sigma=0;
    for (B=0;B<=s;B+=1)
    {
// cout << "B= " << B << " s= " << s << endl;
        t=D+B*B;
        lim=sqrt(t);
        A=2*B;
        if (A==0) A+=1;
        for(;;)
        {
            while (t%A!=0) 
            {
                A+=1;
                if (A>lim) break;
            }

            if (A>lim) break;
            C=t/A;
           
            if (gcd(gcd(A,2*B),C)==1)
            { // output more class group members
                BOOL conj;
                if (2*B>0 && C>A && A>2*B) 
                { 
                    conj=TRUE;
                    cn+=2;
                    if (doit)
                    { 
                        if (!suppress) cout << ".." << flush; 
                    }  
//                    else sigma+=(Flash)2/(Flash)A;
                }
                else
                {
                    conj=FALSE;
                    cn+=1;
                    if (doit)
                    {   
                        if (!suppress) cout << "." << flush; 
                    }
//                    else sigma+=(Flash)1/(Flash)A;
                } 
                if (doit) class_poly(lam,Fi2,A,B,C,D,conj);
            }
            A+=1;
        }        
    }
    return cn;         // class number
}

// check congruence conditions A14.2.1

BOOL isaD(int d,int pm8,Big k)
{
    int i,dm8;
    BOOL sqr;
    dm8=d%8;
    if (k==1 && dm8!=3) return FALSE;
    if ((k==2 || k==3) && dm8==7) return FALSE;
    if (pm8==3 && (dm8==1 || dm8==4 || dm8==5 || dm8==6)) return FALSE;
    if (pm8==5 && dm8%2==0) return FALSE;
    if (pm8==7 && (dm8==1 || dm8==2 || dm8==4 || dm8==5)) return FALSE;
    sqr=FALSE;
    for (i=2;;i++)
    {
        if (d%(i*i)==0)
        {
            sqr=TRUE;
            break;
        }
        if (i*i>d) break;
    }
    if (sqr) return FALSE;
    return TRUE;
}

// Testing for CM discriminants A.14.2.2

Big floor(Big N,Big D)
{
   Big R;
   if (N==0) return 0;
   if (N>0 && D>0) return N/D;
   if (N<0 && D<0) return (-N)/(-D);
   R=N/D;
   if (N%D!=0) R-=1;
   return R;
}

BOOL isacm(Big p,int D,Big &W,Big &V)
{
    Big B2,A,B,C,t,X,Y,ld,delta;
    B=sqrt(p-D,p);
    A=p;
    C=(B*B+D)/p;
    X=1;
    Y=0;
    ld=0;
    while (1)
    {
        if (C>=A)
        {
            B2=2*B;
            if (B2<0) B2=-B2;
            if (A>=B2) break;
        } 
        delta=floor(2*B+C,2*C);

// are we stuck in hopeless loop? This can't happen now - thanks Marcel
        if (delta==0 && ld==0) return FALSE;
        ld=delta;
        t=X;
        X=(delta*X+Y);
        Y=-t;

        t=C;

        C=(A+C*delta*delta-2*B*delta);
        B=(t*delta-B);
        A=t;        
    }
    if (D==11 && A==3)
    {
        t=X;
        X=Y;
        Y=-t;
        B=-B;
        t=C;
        C=A;
        A=t;        
    }      
    if (D==1 || D==3) 
    {
       W=2*X;
       V=2*Y;
       return TRUE;
    }
    else V=0;

    if (A==1) 
    {
        W=2*X;
        return TRUE;
    }

    if (A==4) 
    {
        W=4*X+B*Y;
        return TRUE;
    }

    return FALSE;
}


// Constructing a Curve (and Point in order is prime)
// A.14.4

BOOL get_curve(Complex& lam,Flash *Fi2,ofstream& ofile, Big r, Big other_r,Big ord, int D, Big p, Big W)
{
    Poly polly;
    Big A0,B0,k;
    BOOL unstable;
    char c;
    int i,d,terms,class_number,precision,lh;
    Flash sigma;
    BOOL pord=prime(ord);

    k=r/ord;
    if (!suppress) 
    {
     
        if (D>1000 && D%3==0) cout  << "D= " << D << " (Divisible by 3 - might need extra precision)" << endl;
        else                  cout  << "D= " << D << endl;
        cout << "k= " << k << endl;
    }

    class_number=groups(lam,Fi2,D,FALSE,sigma);
//  get_mip()->RPOINT=ON;
//  cout << "sigma= " << sigma << endl;
//    precision=(int)3.322*((1.364*todouble(sigma)*sqrt((double)D))+class_number/4+5);
//    if (D%2!=0)
//    {
//        lh=0; d=D; while ((d/=2)!=0) lh++; precision=(precision+class_number*lh)/2;         
//    }
//    else if (D%3!=0)
//    {
//        if (D%8==7) {precision/=47; precision+=1;}
//    }
//    precision/=MIRACL;
//    if (precision<32) precision=32;
    
    cout << "class number= " << class_number << endl;

//    cout << "over(?)-estimated precision= " << precision << endl;
 
    cout << "Group order= " << ord << endl;
    cout << "do you want to proceed with this one (Y/N)? " ;
    cin >> c;
    if (c=='N' || c=='n') 
    {
        if (!suppress) cout << "finding a curve..." << endl;
        return FALSE;
    }

    modulo(p);

// A.14.4.1
    
    if (D==1)
    {
        A0=1; B0=0;
    }
    if (D==3)
    {
        A0=0; B0=1;
    }

    if (D!=1 && D!=3)
    {
        Flash f,rem;
        T.clear();
        T.addterm(1,0);   
        if (!suppress) cout << "constructing polynomial";
        groups(lam,Fi2,D,TRUE,sigma);
        terms=degree(T);
        unstable=FALSE;
        for (i=terms;i>=0;i--)
        {
            f=T.coeff(i);
            if (f>0)
                 f+=Flash(1,10000);
            else f-=Flash(1,10000);
            polly.addterm((ZZn)f.trunc(&rem),i);
            if (fabs(rem)>Flash(1,100)) 
            {
                unstable=TRUE; 
                break; 
            }
        }
        T.clear();
        if (!suppress) cout << endl;
        if (unstable) 
        {
            if (!suppress) cout << "Curve abandoned - numerical instability!" << endl;
            if (!suppress) cout << "Curve abandoned - double MIRACL precision and try again!" << endl;
            if (!suppress) cout << "finding a curve..." << endl;
            return FALSE;
        }
        if (!suppress) cout << polly << endl;
    }

    ECn pt,G;
    Big a,b,x,y;
    Big w,eps;
    int at;
    Poly g;

    forever
    {
        if (D!=1 && D!=3)
        {
            if (!suppress) cout << "Factoring polynomial of degree " << degree(polly) << " ....." << endl;
            if (W%2==0)
            {
                ZZn V;
                g=factor(polly,1);
                V=-g.coeff(0);
                V=pow(V,24/(igcd(D,3)*getk(D)));
                V*=pow((ZZn)2,(4*geti(D))/getk(D));
                if (D%2!=0) V=-V;
                A0=(Big)((ZZn)(-3)*(V+64)*(V+16));   
                B0=(Big)((ZZn)2*(V+64)*(V+64)*(V-8));
            }
            else
            {
                Poly V,w,w1,w2,w3,a,b;

                g=factor(polly,3);
                if (D%3!=0)
                    w.addterm(-1,24);
                else
                    w.addterm(-256,8);
                V=w%g;
                w.clear();
                w1=V; w2=V; w3=V;
                w1.addterm(64,0);
                w2.addterm(256,0);
                w3.addterm(-512,0);
                a=w1*w2;
                a.multerm(-3,0);
                a=a%g;
                b=w1*w1*w3;
                b.multerm(2,0);
                b=b%g;

                a=((a*a)*a)%g;
                b=(b*b)%g;
                for (int d=degree(g)-1;d>=0;d--)
                {
                    ZZn t,c=a.coeff(d);
                    if (c!=(ZZn)0)
                    {
                        t=b.coeff(d);
                        A0=(Big)(c*t);
                        B0=(Big)(c*t*t);
                        if (d==1) break;
                    }     
                }
            }
        }

// A.14.4.2
// but try -3 first, followed by small positive values for A

        a=A0;
        b=B0;
        at=-3;
        if (D==3) at=1;
        forever
        {
            if (D==1)
            {
                if (at<100)
                    eps=(ZZn)at/(ZZn)A0;
                else eps=rand(p);
                a=modmult(A0,eps,p);
            }
            if (D==3) 
            {
                if (at<100)
                    eps=(ZZn)at/(ZZn)B0;
                else eps=rand(p);
                b=modmult(B0,eps,p);
            }
            if (D!=1 && D!=3)
            {
                if (at<100)
                { // transform a to be simple if possible
                    w=(ZZn)at/ZZn(A0);
                    if (jacobi(w,p)!=1) 
                    {   
                        if (at==-3) at=1;
                        else at++; 
                        continue;
                    }
                    eps=sqrt(w,p);
                } else eps=rand(p);
                a=modmult(A0,pow(eps,2,p),p);
                b=modmult(B0,pow(eps,3,p),p);   
            } 
            ecurve(a,b,p,MR_PROJECTIVE);
            for (int xc=1;;xc++)
            {
                if (!pt.set((Big)xc)) continue;
                pt*=k;
                if (pt.iszero()) continue;
                break;
            }
            G=pt;                  // check its not the other one...

            if (r!=ord || !(other_r*G).iszero())  
            { 
                pt*=ord;
                if (pt.iszero()) break;
            }

            if (at==-3) at=1;
            else at++;
        }
        if (a==(p-3)) a=-3;
// check MOV condition
// A.12.1

        BOOL MOV=TRUE;
        
        w=1;
        for (i=1;i<100;i++)
        {
            w=modmult(w,p,ord);
            if (w==1) 
            {
               MOV=FALSE;
               if (!suppress) 
               {
                   if (i==1 || pord) cout << "\n*** Failed MOV condition - K = " << i << endl;
                   else                    cout << "\n*** Failed MOV condition - K <= " << i << endl;
               }
               break;
            }
        }
    
        if (!suppress)
        {
            if (MOV)  cout << "MOV condition OK" << endl;
            if (pord) cout << "\nCurve and Point Found" << endl;   
            else      cout << "\nCurve Found" << endl; 
        } 

        cout << "A= " << a << endl;
        cout << "B= " << b << endl;
        G.get(x,y);
        cout << "P= " << p << endl;
        cout << "R= " << ord;
        if (pord) 
        {
            cout << " a " << bits(ord) << " bit prime"  << endl;
            cout << "X= " << x << endl;
            cout << "Y= " << y << endl;
        }
        else            cout << " NOT prime" << endl;
        cout << endl;

        if (D!=1 && D!=3 )
        {
//            polly=polly/g;
//            if (degree(polly)>0)
//            {
                cout << "Try for different random factorisation (Y/N)? ";
                cin >> c;
                if (c=='Y' || c=='y') continue;
//            }
        }
        break;
    }
    if (pord) cout << "\nCurve and Point OK (Y/N)? " ;  
    else      cout << "\nCurve OK (Y/N)? " ;
    cin >> c;
    if (c=='N' || c=='n') 
    {
        if (!suppress) cout << "finding a curve..." << endl;
        return FALSE;
    }
    if (fout)
    {
        ofile << bits(p) << endl;
        mip->IOBASE=16;
        ofile << p << endl;
        ofile << a << endl;
        ofile << b << endl;
        ofile << ord << endl;
        if (pord)
        {
            ofile << x << endl;
            ofile << y << endl;
        }
        mip->IOBASE=10;
    }
    exit(0);
    return TRUE;
}

// Code to parse formula
// This code isn't mine, but its public domain
// Shamefully I forget the source
//
// NOTE: It may be necessary on some platforms to change the operators * and #

#define TIMES '*'
#define RAISE '#'

void eval_power (Big& oldn,Big& n,char op)
{
        int i;
        if (op) n=pow(oldn,toint(n));    // power(oldn,size(n),n,n);
}

void eval_product (Big& oldn,Big& n,char op)
{
        switch (op)
        {
        case TIMES:
                n*=oldn; 
                break;
        case '/':
                n=oldn/n;
                break;
        case '%':
                n=oldn%n;
        }
}

void eval_sum (Big& oldn,Big& n,char op)
{
        switch (op)
        {
        case '+':
                n+=oldn;
                break;
        case '-':
                n=oldn-n;
        }
}

void eval (Big& t)
{
        Big oldn[3];
        Big n;
        int i;
        char oldop[3];
        char op;
        char minus;
        for (i=0;i<3;i++)
        {
            oldop[i]=0;
        }
LOOP:
        while (*s==' ')
        s++;
        if (*s=='-')    /* Unary minus */
        {
        s++;
        minus=1;
        }
        else
        minus=0;
        while (*s==' ')
        s++;
        if (*s=='(' || *s=='[' || *s=='{')    /* Number is subexpression */
        {
        s++;
        eval(t);
        n=t;
        }
        else            /* Number is decimal value */
        {
        for (i=0;s[i]>='0' && s[i]<='9';i++)
                ;
        if (!i)         /* No digits found */
        {
                cout <<  "Error - invalid number" << endl;
                exit (20);
        }
        op=s[i];
        s[i]=0;
        n=atol(s);
        s+=i;
        *s=op;
        }
        if (minus) n=-n;
        do
        op=*s++;
        while (op==' ');
        if (op==0 || op==')' || op==']' || op=='}')
        {
        eval_power (oldn[2],n,oldop[2]);
        eval_product (oldn[1],n,oldop[1]);
        eval_sum (oldn[0],n,oldop[0]);
        t=n;
        return;
        }
        else
        {
        if (op==RAISE)
        {
                eval_power (oldn[2],n,oldop[2]);
                oldn[2]=n;
                oldop[2]=RAISE;
        }
        else
        {
                if (op==TIMES || op=='/' || op=='%')
                {
                eval_power (oldn[2],n,oldop[2]);
                oldop[2]=0;
                eval_product (oldn[1],n,oldop[1]);
                oldn[1]=n;
                oldop[1]=op;
                }
                else
                {
                if (op=='+' || op=='-')
                {
                        eval_power (oldn[2],n,oldop[2]);
                        oldop[2]=0;
                        eval_product (oldn[1],n,oldop[1]);
                        oldop[1]=0;
                        eval_sum (oldn[0],n,oldop[0]);
                        oldn[0]=n;
                        oldop[0]=op;
                }
                else    /* Error - invalid operator */
                {
                        cout <<  "Error - invalid operator" << endl;
                        exit (20);
                }
                }
        }
        }
        goto LOOP;
}

int main(int argc,char **argv)
{
    BOOL good,dir;
    int i,ip,k,D,SD,precision;
    ofstream ofile;

    mip=mirsys(64,0);
    Big p,t;

    argv++; argc--;   
    irand(0L);
    if (argc<1)
    {
      cout << "Incorrect Usage" << endl;
      cout << "Program tries to find Elliptic Curve mod a prime P and point of prime order" << endl;
      cout << "that is a point (X,Y) on the curve y^2=x^3+Ax+B of order R" << endl;
      cout << "where R is large and prime > |P/K|. (K defaults to 256)" << endl;
      cout << "cm <prime number P>" << endl;
      cout << "OR" << endl;
      cout << "cm -f <formula for P>" << endl;
      cout << "e.g. cm -f 2#192-2#64-1" << endl;
      cout << "To suppress the commentary, use flag -s" << endl;
      cout << "(the commentary will make some sense to readers of IEEE 1363 Annex)" << endl;
      cout << "To search downwards for a prime, use flag -d" << endl;
      cout << "To output to a file, use flag -o <filename>" << endl;
      cout << "To set maximum  co-factor size K, use e.g. flag -k8" << endl;
      cout << "To set infinite co-factor size K, use flag -k0" << endl;
      cout << "(In this case the reported R is the number of points on the curve)" << endl;
      cout << "To start searching from a particular D value, use e.g. flag -D10000" << endl;
      cout << "To set MIRACL precision, use e.g. flag -P64 (default -P32)" << endl;
      cout << "If program fails, try again with double precision" << endl;  
      cout << "e.g. cm -f 2#224-2#96+1 -k12 -D400000 -P64 -o common.ecs" << endl;
      cout << "\nFreeware from Shamus Software, Dublin, Ireland" << endl;
      cout << "Full C++ source code and MIRACL multiprecision library available" << endl;
      cout << "Email to mscott@indigo.ie for details" << endl;
      return 0;
    }

    gprime(1000);
    precision=32;
    ip=0;
    k=256;
    SD=1;
    suppress=FALSE;
    fout=FALSE;
    dir=FALSE;
    p=0;
    while (ip<argc)
    {
        if (strcmp(argv[ip],"-f")==0)
        {
            if (p==0)
            {
                ip++;
                s=argv[ip++];
                t=0;
                eval(t);
                p=t;
                continue;
            }
            else
            {
                cout << "Error in command line" << endl;
                return 0;
            }
        }    
        if (strcmp(argv[ip],"-s")==0)
        {
            ip++;
            suppress=TRUE;
            continue;
        }
        if (strcmp(argv[ip],"-d")==0)
        {
            ip++;
            dir=TRUE;
            continue;
        }
        if (strncmp(argv[ip],"-k",2)==0)
        {
             k=atoi(argv[ip]+2);
             ip++;
             continue;
        }
        if (strncmp(argv[ip],"-D",2)==0)
        {
             SD=atoi(argv[ip]+2);
             ip++;
             continue;
        }
        if (strncmp(argv[ip],"-P",2)==0)
        {
             precision=atoi(argv[ip]+2);
             ip++;
             continue;
        }
        if (strcmp(argv[ip],"-o")==0)
        {
            ip++;
            fout=TRUE;
            ofile.open(argv[ip++]);
            continue;
        }
        if (p==0) p=argv[ip++];
        else
        {
            cout << "Error in command line" << endl;
            return 0;
        }
    }

    if (!prime(p))
    {
        int incr=0;
        cout << "That number is not prime!" << endl;
        if (dir)
        {
            cout << "Looking for next lower prime" << endl;
            p-=1; incr++;
            while (!prime(p)) { p-=1;  incr++; }
            cout << "Prime P = P-" << incr << endl;
        }
        else
        {
            cout << "Looking for next higher prime" << endl;
            p+=1; incr++;
            while (!prime(p)) { p+=1;  incr++; }
            cout << "Prime P = P+" << incr << endl;
        }
        cout << "Prime P = " << p << endl;
    }
    if (p<10)
    {
        cout << "Prime is too small - use another method!" << endl;
        return 0;
    }
    if (bits(p)>1024)
    {
        cout << "Prime is too big - sorry" << endl;
        return 0;
    }

    mirexit();

    if (precision<16) precision=16;

    mip=mirsys(precision,0);     // restart with new precision
    gprime(100000);

    Big W,V,K,r1,r2,ord,rmin;
    Complex lam;

    Flash Fi2[7];
    Flash pi24;
    
    if (k==0) rmin=1;
    else      rmin=(p-2*sqrt(p))/k;
    if (rmin==0) 
    {
        cout << "Bad k co-factor value" << endl;
        return 0;
    }

    W=sqrt(p)+1;
    K=(W*W)/rmin;

    if (!suppress) cout << "P mod 8 = " << p%8 << endl;
    if (!suppress) cout << "P is " << bits(p) << " bits long" << endl;
    if (!suppress) cout << "precomputations..." << endl;

    pi24=pi()/(Flash)24; 
    lam=exp(Complex((Flash)0,pi24));

    Fi2[0]=1;
    Fi2[2]=pow((Flash)2,Flash(-1,3));
    Fi2[3]=pow((Flash)2,Flash(-1,2));
    Fi2[6]=Flash(1,2);

    if (!suppress) cout << "finding a curve..." << endl;

    for (D=SD;;D++)
    {
        if (!isaD(D,p%8,K)) continue;
        if (jacobi(p-D,p)==-1) continue;
        good=TRUE;
// A.14.2.3
        for (i=1;;i++)
        {
            int sp=mip->PRIMES[i];
            if (D==sp || sp*sp>D) break;
            if (D%sp==0 && jacobi(p,(Big)sp)==-1)
            {
                good=FALSE;
                break;
            }
        }
        if (!good) continue;
        if (!isacm(p,D,W,V)) continue;

        r1=p+1+W;
        r2=p+1-W;

        if (k==0) ord=r1;
        else      ord=trial_divide(r1);
        if (ord==1) ord=r1;
        if (ord>rmin && (k==0 || prime(ord))) get_curve(lam,Fi2,ofile,r1,r2,ord,D,p,W);

        if (k==0) ord=r2;
        else      ord=trial_divide(r2);
        if (ord==1) ord=r2;
        if (ord>rmin && (k==0 || prime(ord))) get_curve(lam,Fi2,ofile,r2,r1,ord,D,p,W);

        if (D==1)
        {
            r1=p+1+V;
            r2=p+1-V;
            if (k==0) ord=r1;
            else      ord=trial_divide(r1);
            if (ord==1) ord=r1;
            if (ord>rmin && (k==0 || prime(ord))) get_curve(lam,Fi2,ofile,r1,r2,ord,D,p,W);

            if (k==0) ord=2;
            else      ord=trial_divide(r2);
            if (ord==1) ord=r2;
            if (ord>rmin && (k==0 || prime(ord))) get_curve(lam,Fi2,ofile,r2,r1,ord,D,p,W);
            
        }
        if (D==3)
        {
            r1=p+1+(W+3*V)/2;
            r2=p+1-(W+3*V)/2;
            if (k==0) ord=r1;
            else ord=trial_divide(r1);
            if (ord==1) ord=r1;
            if (ord>rmin && (k==0 || prime(ord))) get_curve(lam,Fi2,ofile,r1,r2,ord,D,p,W);

            if (k==0) ord=r2;
            else ord=trial_divide(r2);
            if (ord==1) ord=r2;
            if (ord>rmin && (k==0 || prime(ord))) get_curve(lam,Fi2,ofile,r2,r1,ord,D,p,W);

            r1=p+1+(W-3*V)/2;
            r2=p+1-(W-3*V)/2;
            if (k==0) ord=r1;
            else ord=trial_divide(r1);
            if (ord==1) ord=r1;
            if (ord>rmin && (k==0 || prime(ord))) get_curve(lam,Fi2,ofile,r1,r2,ord,D,p,W);

            if (k==0) ord=r2;
            else ord=trial_divide(r2);
            if (ord==1) ord=r2;
            if (ord>rmin && (k==0 || prime(ord))) get_curve(lam,Fi2,ofile,r2,r1,ord,D,p,W);
        }
    }
    cout << "No satisfactory curve found" << endl;
    return 0;
}


