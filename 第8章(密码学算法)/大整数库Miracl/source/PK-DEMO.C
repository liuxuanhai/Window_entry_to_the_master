/*
 *   Example program demonstrates 1024 bit Diffie-Hellman, El Gamal and RSA
 *   and 168 bit Elliptic Curve Diffie-Hellman 
 *
 *   Copyright (c) 1988-2001 Shamus Software Ltd.
 */

#include <stdio.h>
#include <miracl.h>
#include <time.h>

/* large 1024 bit prime p for which (p-1)/2 is also prime */
char *primetext=
"155315526351482395991155996351231807220169644828378937433223838972232518351958838087073321845624756550146945246003790108045940383194773439496051917019892370102341378990113959561895891019716873290512815434724157588460613638202017020672756091067223336194394910765309830876066246480156617492164140095427773547319";

/* Use elliptic curve of the form y^2=x^3-3x+B */

/* NIST p224 bit elliptic curve prime 2#224-2#96+1 */

char *ecp="FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000001";

/* elliptic curve parameter B */

char *ecb="B4050A850C04B3ABF54132565044B0B7D7BFD8BA270B39432355FFB4";

/* elliptic curve - point of prime order (x,y) */

char *ecx="F7D13920ECA3006B6FA69B37A1178888E7E7DAB446ACF673331DD48A";
char *ecy="E312F42445ED3C782899948B8379FAE620819FA084EF617C185FA6DC";


char *text="MIRACL - Fastest multi-precision library in the World!\n";

int main()
{
    int ia,ib;
    unsigned long seed;
    epoint *g,*ea,*eb;
    big a,b,p,q,n,p1,q1,phi,pa,pb,key,e,d,dp,dq,t,m,c,x,y,k;
    big primes[2],pm[2];
    big_chinese ch;
    miracl *mip;
#ifndef MR_NOFULLWIDTH   
    mip=mirsys(100,0);
#else
    mip=mirsys(100,MAXBASE);
#endif
    a=mirvar(0);
    b=mirvar(0);
    p=mirvar(0);
    q=mirvar(0);
    n=mirvar(0);
    p1=mirvar(0);
    q1=mirvar(0);
    phi=mirvar(0);
    pa=mirvar(0);
    pb=mirvar(0);
    key=mirvar(0);
    e=mirvar(0);
    d=mirvar(0);
    dp=mirvar(0);
    dq=mirvar(0);
    t=mirvar(0);
    m=mirvar(0);
    c=mirvar(0);
    pm[0]=mirvar(0);
    pm[1]=mirvar(0);
    x=mirvar(0);
    y=mirvar(0);
    k=mirvar(0);
    time((time_t *)&seed);
    irand(seed);   /* change parameter for different values */

    printf("First Diffie-Hellman Key exchange .... \n");

    cinstr(p,primetext);

/* offline calculations could be done quicker using Brickell's idea
   - See brick.c. Note use of "truncated exponent" of 160 bits -  
   could be output of hash function SHA (see mrshs.c)               */

    printf("\nAlice's offline calculation\n");        
    bigdig(160,2,a);

/* 3 generates the sub-group of prime order (p-1)/2 */

    powltr(3,a,p,pa);

    printf("Bob's offline calculation\n");        
    bigdig(160,2,b);
    powltr(3,b,p,pb);

    printf("Alice calculates Key=\n");
    powmod(pb,a,p,key);
    cotnum(key,stdout);

    printf("Bob calculates Key=\n");
    powmod(pa,b,p,key);
    cotnum(key,stdout);

    printf("Alice and Bob's keys should be the same!\n");

/* 
   Now Elliptic Curve version of the above.
   Curve is y^2=x^3+Ax+B mod p, where A=-3, B and p as above 
   "Primitive root" is the point (x,y) above, which is of large prime order q. 
   In this case actually
   q= FFFFFFFFFFFFFFFFFFFFFFFFFFFF16A2E0B8F03E13DD29455C5C2A3D
 
*/

    printf("\nLets try that again using elliptic curves .... \n");
    convert(-3,a);
    mip->IOBASE=16;
    cinstr(b,ecb);
    cinstr(p,ecp);      
    ecurve_init(a,b,p,MR_PROJECTIVE);

    g=epoint_init();
    cinstr(x,ecx);
    cinstr(y,ecy);
    mip->IOBASE=10;
    epoint_set(x,y,0,g);
    ea=epoint_init();
    eb=epoint_init();
    epoint_copy(g,ea);
    epoint_copy(g,eb);

    printf("Alice's offline calculation\n");        
    bigdig(160,2,a);
    ecurve_mult(a,ea,ea);
    ia=epoint_get(ea,pa,pa); /* <ia,pa> is compressed form of public key */

    printf("Bob's offline calculation\n");        
    bigdig(160,2,b);
    ecurve_mult(b,eb,eb);
    ib=epoint_get(eb,pb,pb); /* <ib,pb> is compressed form of public key */

    printf("Alice calculates Key=\n");
    epoint_set(pb,pb,ib,eb); /* decompress eb */
    ecurve_mult(a,eb,eb);
    epoint_get(eb,key,key);
    cotnum(key,stdout);

    printf("Bob calculates Key=\n");
    epoint_set(pa,pa,ia,ea); /* decompress ea */
    ecurve_mult(b,ea,ea);
    epoint_get(ea,key,key);
    cotnum(key,stdout);

    printf("Alice and Bob's keys should be the same! (but much smaller)\n");

    epoint_free(g);
    epoint_free(ea);
    epoint_free(eb);

/* El Gamal's Method */

    printf("\nTesting El Gamal's public key method\n");
    cinstr(p,primetext);
    bigdig(160,2,x);    /* x<p */
    powltr(3,x,p,y);    /* y=3^x mod p*/
    decr(p,1,p1);
    mip->IOBASE=256;
    cinstr(m,text);
    mip->IOBASE=10;
    do 
    {
        bigdig(160,2,k);
    } while (egcd(k,p1,t)!=1);
    powltr(3,k,p,a);   /* a=3^k mod p */
    powmod(y,k,p,b);
    mad(b,m,m,p,p,b);  /* b=m*y^k mod p */
    printf("Ciphertext= \n");
    cotnum(a,stdout);
    cotnum(b,stdout);

    zero(m);           /* proof of pudding... */
  
    subtract(p1,x,t);
    powmod(a,t,p,m);
    mad(m,b,b,p,p,m);  /* m=b/a^x mod p */

    printf("Plaintext= \n");
    mip->IOBASE=256;
    cotnum(m,stdout);
    mip->IOBASE=10;

/* RSA. Generate primes p & q. Use e=65537, and find d=1/e mod (p-1)(q-1) */

    printf("\nNow generating 512-bit random primes p and q\n");
    do 
    {
        bigdig(512,2,p);
        if (subdivisible(p,2)) incr(p,1,p);
        while (!isprime(p)) incr(p,2,p);

        bigdig(512,2,q);
        if (subdivisible(q,2)) incr(q,1,q);
        while (!isprime(q)) incr(q,2,q);

        multiply(p,q,n);      /* n=p.q */

        lgconv(65537L,e);
        decr(p,1,p1);
        decr(q,1,q1);
        multiply(p1,q1,phi);  /* phi =(p-1)*(q-1) */
    } while (xgcd(e,phi,d,d,t)!=1);

    cotnum(p,stdout);
    cotnum(q,stdout);
    printf("n = p.q = \n");
    cotnum(n,stdout);

/* set up for chinese remainder thereom */
    primes[0]=p;
    primes[1]=q;

    crt_init(&ch,2,primes);
    copy(d,dp);
    copy(d,dq);
    divide(dp,p1,p1);   /* dp=d mod p-1 */
    divide(dq,q1,q1);   /* dq=d mod q-1 */
    mip->IOBASE=256;
    cinstr(m,text);
    mip->IOBASE=10;
    printf("Encrypting test string\n");
    powmod(m,e,n,c);
    printf("Ciphertext= \n");
    cotnum(c,stdout);

    zero(m);

    printf("Decrypting test string\n");
    powmod(c,dp,p,pm[0]);    /* get result mod p */
    powmod(c,dq,q,pm[1]);    /* get result mod q */
    crt(&ch,pm,m);           /* combine them using CRT */

    printf("Plaintext= \n");
    mip->IOBASE=256;
    cotnum(m,stdout);
    crt_end(&ch);
    return 0;
}

