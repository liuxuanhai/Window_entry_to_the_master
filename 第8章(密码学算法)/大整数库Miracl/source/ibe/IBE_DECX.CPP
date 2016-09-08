/*
   Boneh & Franklin's Identity Based Encryption 
  
   y^2=x^3+x version
   Using this curve has certain advantages. In particular the "denominator"
   that arises in the context of Miller's algorithm is in Fp, and hence 
   "disappears" with the final exponentiation. So it can be left out 
   altogther. 

   Decryption phase

   Decrypts a file <filename>.ibe
   Finds the session key by IBE decrypting the file <filename>.key
   Uses this session key to AES decrypt the file.

   Outputs to the console

   NOTE: Uses Tate Pairing only
   NOTE: New fast Tate pairing algorithm

   Compile as 
   cl /O2 /GX /DBIGS=18 ibe_dec.cpp zzn2.cpp big.cpp monty.cpp elliptic.cpp miracl.lib
   where miracl is built using the Comba method.

*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <elliptic.h>
#include <monty.h>
#include <ebrick.h>
#include "zzn2.h"

using namespace std;

#define PBITS 512
#define QBITS 160

// Using SHA-1 as basic hash algorithm

#define HASH_LEN 20

//
// Define one or the other of these
//
// Which is faster depends on the I/M ratio - See imratio.c
// Roughly if I/M ratio > 16 use PROJECTIVE, otherwise use AFFINE
//

// #define AFFINE
#define PROJECTIVE

// define this if group order q is "simple" = 2^159+1^17+1
#define SIMPLE

//
// Tate Pairing Code
//
// Extract ECn point in internal ZZn format
//

void extract(ECn& A,ZZn& x,ZZn& y)
{ 
    x=(A.get_point())->X;
    y=(A.get_point())->Y;
}

void extract(ECn& A,ZZn& x,ZZn& y,ZZn& z)
{ 
    big t;
    x=(A.get_point())->X;
    y=(A.get_point())->Y;
    t=(A.get_point())->Z;
    if (A.get_status()!=MR_EPOINT_GENERAL) z=1;
    else                                   z=t;
}

//
// Line from A to destination C. Let A=(x,y)
// Line Y-slope.X-c=0, through A, so intercept c=y-slope.x
// Line Y-slope.X-y+slope.x = (Y-y)-slope.(X-x) = 0
// Now evaluate at Q -> return (Qy-y)-slope.(Qx-x)
//

ZZn2 line(ECn& A,ECn& C,ZZn& slope,ZZn& Qx,ZZn2& Qy)
{ 
    ZZn2 w=Qy;
    ZZn x,y,z,t,m=Qx;
#ifdef AFFINE
    extract(A,x,y);
    w-=y; m-=x; m*=slope; w-=m;  // 1 ZZn mul
#endif
#ifdef PROJECTIVE
    extract(A,x,y,z);               
    x*=z; t=z; z*=z; z*=t;       // 9 ZZn muls   
    m*=z; m-=x; m*=slope;
    w*=z; w-=y; 
    extract(C,x,y,z);
    w*=z; w-=m;
#endif
    return w;
}

//
// Add A=A+B  (or A=A+A) 
//
// AFFINE doubling     - 8 ZZn muls, plus 1 inversion
// AFFINE adding       - 7 ZZn muls, plus 1 inversion
//
// PROJECTIVE doubling - 20 ZZn muls
// PROJECTIVE adding   - 28 ZZn muls
//

void g(ECn& A,ECn& B,ZZn& Qx,ZZn2& Qy,ZZn2& num)
{
    ZZn  lam;
    ZZn2 u;
    ECn P=A;
    big ptr;

    ptr=A.add(B);
    if (ptr==NULL)     { num=0; return; }
    lam=ptr;

    if (A.iszero()) return; 
    
    u=line(P,A,lam,Qx,Qy);

    num*=u;                // 3 ZZn muls  
}

//
// Tate Pairing 
//
// Special optimized and deterministic version of Tate Pairing algorithm 
// P and Q(x,y) linearly independent, that is P!=r.Q for any r, and odd order q.
// If P & Q are linearly dependent it might fail, but this will be detected.
//
// P & Q(x,y) are both points of order q. 
// Note that P is a point on the curve over Fp, Q(x,y) a point on the 
// quadratic extension field Fp^2
//

BOOL fast_tate_pairing(ECn& P,ZZn& Qx,ZZn2& Qy,Big& q,ZZn2& res)
{ 
    int i;
    Big p;
    ECn A;

    res=1; 

// q.P = 2^17*(2^142.P +P) + P

    A=P;    // reset A
    for (i=0;i<142;i++)
    {
        res*=res;          
        g(A,A,Qx,Qy,res);            // 10 ZZn muls + 1 inverse 
        if (res==0) return FALSE;
    }                                // 22 ZZn muls (Projective)
    g(A,P,Qx,Qy,res);                // 9  ZZn muls + 1 inverse 
    if (res==0) return FALSE;
    for (i=0;i<17;i++)               // 28 ZZn muls (Projective)
    {
        res*=res;          
        g(A,A,Qx,Qy,res);            // 10 ZZn muls + 1 inverse 
        if (res==0) return FALSE;
    }                                // 22 ZZn muls (Projective)
    g(A,P,Qx,Qy,res);                // 9 ZZn muls + 1 inverse 

    if (res.iszero()) return FALSE;
    if (!A.iszero()) return FALSE;

    p=get_modulus();         // get p
    res= pow(res,(p+1)/q);   // raise to power of (p^2-1)/q
    res=conj(res)/res;
    if (res.isunity()) return FALSE;

    return TRUE;   
}

//
// ecap(.) function
//

BOOL ecap(ECn& P,ECn& Q,Big& order,ZZn2& res)
{
    ZZn  Qx;
    ZZn2 Qy;
    Big xx,yy;          /* apply distortion map x,y=-x,iy */

    Q.get(xx,yy);       /* Q*=[(-1,0),(0,1)] */
    Qx=-xx;
    Qy.set((ZZn)0,yy);

    if (fast_tate_pairing(P,Qx,Qy,order,res)) return TRUE;
    return FALSE;
}

//
// Hash functions
// 

int H2(ZZn2 x,char *s)
{ // Hash an Fp2 to an n-byte string s[.]. Return n
    sha sh;
    Big a,b;
    int m;

    shs_init(&sh);
    x.get(a,b);
    while (a>0)
    {
        m=a%256;
        shs_process(&sh,m);
        a/=256;
    }
    while (b>0)
    {
        m=b%256;
        shs_process(&sh,m);
        b/=256;
    }
    shs_hash(&sh,s);

    return HASH_LEN;
}

Big H3(char *x1,char *x2)
{
    sha sh;
    char h[HASH_LEN];
    Big a;
    int i;

    shs_init(&sh);
    for (i=0;i<HASH_LEN;i++)
        shs_process(&sh,x1[i]);
    for (i=0;i<HASH_LEN;i++)
        shs_process(&sh,x2[i]);
    shs_hash(&sh,h);
    a=from_binary(HASH_LEN,h);
    return a;
}

void H4(char *x,char *y)
{ // hashes y=h(x)
    int i;
    sha sh;
    shs_init(&sh);
    for (i=0;i<HASH_LEN;i++)
        shs_process(&sh,x[i]);
    shs_hash(&sh,y);
}

void strip(char *name)
{ /* strip extension off filename */
    int i;
    for (i=0;name[i]!='\0';i++)
    {
        if (name[i]!='.') continue;
        name[i]='\0';
        break;
    }
}

int main()
{
    miracl *mip=mirsys(18,0);    // thread-safe ready.  (36,0) for 1024 bit p
    ifstream common("commonx.ibe");
    ifstream private_key("privatex.ibe");
    ifstream key_file,ciphertext;
    ECn U,P,rP,Ppub,Qid,Did,infinity;
    char key[HASH_LEN],pad[HASH_LEN],rho[HASH_LEN],V[HASH_LEN],W[HASH_LEN];
    char ifname[100],ch,iv[16];
    ZZn2 gid,w;
    Big s,p,q,r,cof,x,y;
    int i,bits;
    aes a;

// DECRYPT

    common >> bits;
    mip->IOBASE=16;
    common >> p >> q;

    cof=(p+1)/q;

    common >> x >> y;
    EBrick B(x,y,(Big)1,(Big)0,p,QBITS);   // precomputation based on fixed P

#ifdef AFFINE
    ecurve(1,0,p,MR_AFFINE);
#endif
#ifdef PROJECTIVE
    ecurve(1,0,p,MR_PROJECTIVE);
#endif

    P.set(x,y);

    common >> x >> y;
    Ppub.set(x,y);

// get private key

    private_key >> x;
    private_key >> y;

    Did.set(x,y);

// figure out where input is coming from

    cout << "file to be decoded = ";
    cin >> ifname;

    strip(ifname);          // open key file
    strcat(ifname,".key");
    key_file.open(ifname,ios::in);
    if (!key_file)
    {
        cout << "Key file " << ifname << " not found\n";
        return 0;
    }

    strip(ifname);          // open ciphertext
    strcat(ifname,".ibe");
    ciphertext.open(ifname,ios::binary|ios::in);
    if (!ciphertext)
    {
        cout << "Unable to open file " << ifname << "\n";
        return 0;
    }

    mip->IOBASE=16;

// get file info

    key_file >> x;
    key_file >> y;

    if (!U.set(x,y)) cout << "U is not on the curve!" << endl;   

    key_file >> x;
    to_binary(x,HASH_LEN,V,TRUE);
    key_file >> x;
    to_binary(x,HASH_LEN,W,TRUE);

    mip->IOBASE=10;

// DECRYPT - extract session key

//    Not needed as ecap calculates implicit q*U
//
//    if (q*U!=infinity) 
//    {
//        cout << "Ciphertext rejected" << endl;
//        exit(0);
//    }
    cout << "Decrypting message" << endl;    

    if (!ecap(U,Did,q,w))
    {
        cout << "Ciphertext rejected" << endl;
        exit(0);
    }

    H2(w,pad);    
    for (i=0;i<HASH_LEN;i++) rho[i]=V[i]^pad[i];

    H4(rho,pad);
    for (i=0;i<HASH_LEN;i++) key[i]=W[i]^pad[i];

    r=H3(rho,key);
    
    B.mul(r,x,y);        // r*P
    rP.set(x,y);

    if (U!=rP)
    {
        cout << "Ciphertext rejected" << endl;
        exit(0);
    }
    
//
// Use session key to decrypt file
//

    for (i=0;i<16;i++) iv[i]=i;  // Set CFB IV
    aes_init(&a,MR_CFB1,16,key,iv);

    forever
    { // decrypt input ..
        ciphertext.get(ch);
        if (ciphertext.eof()) break;
        aes_decrypt(&a,&ch);
        cout << ch;
    }
    aes_end(&a);

    return 0;
}


