/* program to find smallest irreducible polynomial */
/* cl /O2 /GX irred.cpp polymod.cpp poly.cpp big.cpp monty.cpp ms32.lib 
 */

#include <iostream>
#include <fstream>
#include <cstring>
#include "polymod.h"

using namespace std;

// #define BINOMIAL

Miracl precision=100;

// Code to parse formula in command line
// This code isn't mine, but its public domain
// Shamefully I forget the source
//
// NOTE: It may be necessary on some platforms to change the operators * and #
//

#define TIMES '*'
#define RAISE '#'

Big tt;
static char *ss;

void eval_power (Big& oldn,Big& n,char op)
{
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

void eval (void)
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
        while (*ss==' ')
        ss++;
        if (*ss=='-')    /* Unary minus */
        {
        ss++;
        minus=1;
        }
        else
        minus=0;
        while (*ss==' ')
        ss++;
        if (*ss=='(' || *ss=='[' || *ss=='{')    /* Number is subexpression */
        {
        ss++;
        eval ();
        n=tt;
        }
        else            /* Number is decimal value */
        {
        for (i=0;ss[i]>='0' && ss[i]<='9';i++)
                ;
        if (!i)         /* No digits found */
        {
                cout <<  "Error - invalid number" << endl;
                exit (20);
        }
        op=ss[i];
        ss[i]=0;
        n=atol(ss);
        ss+=i;
        *ss=op;
        }
        if (minus) n=-n;
        do
        op=*ss++;
        while (op==' ');
        if (op==0 || op==')' || op==']' || op=='}')
        {
        eval_power (oldn[2],n,oldop[2]);
        eval_product (oldn[1],n,oldop[1]);
        eval_sum (oldn[0],n,oldop[0]);
        tt=n;
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

int main(int argc,char ** argv)
{
    ofstream ofile;
    int ip,i,j,k,m,s,M,Base;
    BOOL gotP,ir,fout,first;
    Big w,p,t,xx[12];
    miracl *mip=&precision;
    argc--; argv++;
   
    if (argc<1)
    {
        cout << "incorrect usage" << endl;
        cout << "Program finds simplest irreducible polynomial with respect" << endl;
        cout << "to a given prime modulus, of the form x^k+x^2+n" << endl;
        cout << "irred <prime modulus P> <M>" << endl;
        cout << "OR" << endl;
        cout << "irred <formula for P> <M>" << endl;
        cout << "To input P in Hex, precede with -h" << endl;
        cout << "To output to a file, use flag -o <filename>" << endl;
        cout << "e.g. irred -f 2#192-2#64-1 6 -o zzn6.dat" << endl;
        return 0;
    }

    fout=FALSE;
    Base=10;
    ip=0;
    M=0;
    gotP=FALSE;
    while (ip<argc)
    { 
        if (!gotP && strcmp(argv[ip],"-f")==0)
        {
            ip++;
            if (!gotP && ip<argc)
            {

                ss=argv[ip++];
                tt=0;
                eval();
                p=tt;
                gotP=TRUE;
                continue;
            }
            else
            {
                cout << "Error in command line" << endl;
                return 0;
            }
        }

        if (strcmp(argv[ip],"-o")==0)
        {
            ip++;
            if (ip<argc)
            {
                fout=TRUE;
                ofile.open(argv[ip++]);
                continue;
            }
            else
            {
                cout << "Error in command line" << endl;
                return 0;
            }
        }

        if (strcmp(argv[ip],"-h")==0)
        {
            ip++;
            Base=16;
            continue;
        }
        if (!gotP)
        {
            mip->IOBASE=Base;
            p=argv[ip++];
            mip->IOBASE=10;
            gotP=TRUE;
            continue;
        }
        if (gotP)
        {
            M=atoi(argv[ip++]);
            continue;
        }
        cout << "Error in command line" << endl;
        return 0;
    }
    if (!gotP || M<3)
    {
        cout << "Error in command line" << endl;
        return 0;
    }

    if (!prime(p)) 
    {
        cout << "This number is not prime!" << endl;
        exit(0);
    }
    if (M>12)
    {
        cout << "M is too big!" << endl;
        exit(0);
    }
    modulo(p);

    PolyMod h,g,x;
    Poly f;
    
    x.addterm(1,1);

    i=1; j=2; m=1; first=FALSE;
    forever
    {
        g=x;
    
        f.clear();             
        f.addterm(i,0); 
#ifndef BINOMIAL
        f.addterm(m,j); 
#endif 
        f.addterm(1,M);

        setmod(f);
    
        ir=TRUE;
        for (k=1;k<=M/2;k++)
        {
            g=pow(g,p);
            h=gcd(g-x);
            if (!isone(h)) { ir=FALSE; break; }
        }
        if (ir) break;

        if (first) 
        {
#ifndef BINOMIAL
            if (m==1) 
            {
                m=-1;
                continue; 
            }
            m=1; 
#endif
            j=j+1;
            if (j==M)
            {
                j=1;
                if (i<0) first=FALSE;
                else     i=-1;
            }
            continue;
        }

        if (i>0) i=(-i); 
        else { i=(-i); i++; }
    }

    cout << "\nirreducible polynomial P(x) = " << f << endl << endl; 

    if (fout) 
    {
        ofile << i << endl;
        ofile << j << endl;
    }
    g=x;
    g=pow(g,p);
    if (fout)
        for (i=0;i<M;i++) ofile << g.coeff(i) << endl;

    for (j=2;j<M;j++)
        for (i=0;i<M;i++) ofile << pow(g,j).coeff(i) << endl;

    s=0;                         // HAC 3.34 Step 3
    t=pow(p,M)-1;
    while (t%2==0) { t/=2; s++; }
    if (fout) ofile << s << endl;
    w=t;
    for (i=0;i<M;i++)
    {
        if (fout) ofile << w%p << endl;
        w/=p;
    }
    w=(t+1)/2;
    for (i=0;i<M;i++)
    {
        if (fout) ofile << w%p << endl;
        w/=p;
    }

/*
    for (i=1;i<6;i++)
    {
        cout << i << "*p^2 should be x^" << i << " is " << pow(x,i*p*p) << endl;
    }

    for (i=1;i<6;i++)
    {
        cout << i << "*p^3 should be x^" << i << " is " << pow(x,i*p*p*p) << endl;
    }

    for (i=1;i<8;i++)
    {
        cout << i << "*p^4 should be x^" << i << " is " << pow(x,i*p*p*p*p) << endl;
    }

    for (i=1;i<10;i++)
    {
        cout << i << "*p^5 should be x^" << i << " is " << pow(x,i*p*p*p*p*p) << endl;
    }

    for (i=1;i<12;i++)
    {
        cout << i << "*p^6 should be x^" << i << " is " << pow(x,i*p*p*p*p*p*p) << endl;
    }
*/
    return 0;
}

