/*
   Boneh & Franklin's Identity Based Encryption y^2=x^3+x version
   
   Set-up phase

   After this program has run the file commonx.ibe contains

   <Size of prime modulus in bits>
   <Prime p>
   <Prime q (divides p+1) >
   <Point P - x coordinate>
   <Point P - y coordinate>
   <Point Ppub - x coordinate>
   <Point Ppub - y coordinate>

   The file masterx.ibe contains

   <The master secret s>

   NOTE: define SIMPLE below to use a "simple" fixed group order q
         with the minimum number of 1's. Here we use q=2^159+2^17+1

 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <elliptic.h>
#include <monty.h>
#include "zzn2.h"

using namespace std;

//
// Set parameter sizes. For example change PBITS to 1024
//

#define PBITS 512
#define QBITS 160

#define SIMPLE

Miracl precision(36,0);  // increase if PBITS increases. (36,0) for 1024 bit p

int main()
{
    ofstream common("commonx.ibe");
    ofstream master("masterx.ibe");
    ECn P,Ppub;
    Big s,p,q,t,n,cof,x,y;
    long seed;
    miracl *mip=&precision;

    cout << "Enter 9 digit random number seed  = ";
    cin >> seed;
    irand(seed);

// SET-UP

#ifdef SIMPLE

    q=pow((Big)2,159)+pow((Big)2,17)+1;

#else

// generate random q 

    forever
    {
        n=rand(QBITS-1,2);  // 159 bit number, base 2 
        q=2*n+1;            // 160 bit
        while (!prime(q)) q+=2;
        if (bits(q)>QBITS) continue;
        break;
    }

#endif

    cout << "q= " << q << endl;

// generate p 
    t=(pow((Big)2,PBITS)-1)/(2*q);
    s=(pow((Big)2,PBITS-1)-1)/(2*q);
    forever 
    {
        n=rand(t);
        if (n<s) continue;
        p=2*n*q-1;
        if (p%12!=11) continue;  // must be 2 mod 3, also 3 mod 4
        if (prime(p)) break;
    } 
    cout << "p= " << p << endl;
    cout << "p%4= " << p%4 << endl;

    cof=2*n; 

    ecurve(1,0,p,MR_PROJECTIVE);    // elliptic curve y^2=x^3+x mod p

//
// Choosing an arbitrary P ....
//
    forever
    {
        while (!P.set(randn())) ;
        P*=cof;
        if (!P.iszero()) break;
    }

    cout << "Point P= " << P << endl;

//
// Pick a random master key s 
//    
    s=rand(q);
    Ppub=s*P;
    cout << "Secret s= " << s << endl;
    cout << "Point Ppub= " << Ppub << endl;

    common << PBITS << endl;
    mip->IOBASE=16;
    common << p << endl;
    common << q << endl;
    P.get(x,y);
    common << x << endl;
    common << y << endl;
    Ppub.get(x,y);
    common << x << endl;
    common << y << endl;

    master << s << endl;    

    return 0;
}

