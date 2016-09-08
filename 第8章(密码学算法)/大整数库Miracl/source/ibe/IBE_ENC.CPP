/*
   Boneh & Franklin's Identity Based Encryption
  
   Encryption phase
  
   Generates a random AES session key, and uses it to encrypt a file.
   Outputs ciphertext <filename>.ibe.

   The session key is IBE encrypted, and written to <filename>.key

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

// define SIMPLE if using fixed group order q = 2^159+2^17+1
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

ZZn2 line(ECn& A,ECn& C,ZZn& slope,ZZn2& Qx,ZZn& Qy)
{ 
    ZZn2 n=Qx;
    ZZn x,y,z,t,w=Qy;
#ifdef AFFINE
    extract(A,x,y);
    n-=x; n*=slope;            // 2 ZZn muls
    w-=y; n-=w;
#endif
#ifdef PROJECTIVE
    extract(A,x,y,z);
    x*=z; t=z; z*=z; z*=t;          
    n*=z; n-=x;                // 9 ZZn muls
    w*=z; w-=y; 
    extract(C,x,y,z);
    w*=z; n*=slope; n-=w;                     
#endif
    return n;
}

//
// Vertical line through point A
//

ZZn2 vertical(ECn& A,ZZn2& Qx)
{
    ZZn2 n=Qx;
    ZZn x,y,z;
#ifdef AFFINE
    extract(A,x,y);
    n-=x;
#endif
#ifdef PROJECTIVE
    extract(A,x,y,z);
    z*=z;                    
    n*=z; n-=x;                // 3 ZZn muls
#endif
    return n;
}

//
// Add A=A+B  (or A=A+A) or
// Sub A=A-B
// Bump up num and denom
//
// AFFINE doubling     - 12 ZZn muls, plus 1 inversion
// AFFINE adding       - 11 ZZn muls, plus 1 inversion
//
// PROJECTIVE doubling - 26 ZZn muls
// PROJECTIVE adding   - 34 ZZn muls
//

#define ADD 1
#define SUB 2

void g(ECn& A,ECn& B,ZZn2& Qx,ZZn& Qy,ZZn2& num,ZZn2& denom,int as,BOOL first)
{
    ZZn  lam,mQy;
    ZZn2 d,u;
    big ptr;
    ECn P=A;

    if (as==ADD)
    { // Evaluate line from A, and then evaluate vertical through destination
        ptr=A.add(B);
        if (ptr==NULL)    { num=0; return; }
        else lam=ptr;

        if (A.iszero())   { u=vertical(P,Qx); d=1; }
        else
        {
            u=line(P,A,lam,Qx,Qy);
            d=vertical(A,Qx);
        }
    }
    else // as==SUB
    { // Evaluate Vertical at A, and then line from A to destination
      // (Note swap num and denom, Qy=-Qy, process lines "backwards")
        u=vertical(A,Qx);
        ptr=A.sub(B);
        if (ptr==NULL)    { num=0; return; }
        else lam=ptr;

        if (A.iszero())   { d=u;  } 
        else
        {
            mQy=-Qy;
            d=line(P,A,lam,Qx,mQy);
        }
    }

    if (first) {num= u; denom= d; }
    else       {num*=u; denom*=d; }   // 6 ZZn muls  
}

//
// Tate Pairing - note ha -> numerator, had -> denominator
// Trying to minimize number of modular inversions
//
// Special optimized and deterministic version of Tate Pairing algorithm 
// Requires that the point (0,1) is on the curve AND
// P and Q(x,y) linearly independent, that is P!=r.Q for any r, and odd order q.
// If P & Q are linearly dependent it might fail, but this will be detected.
//
// Sketch of proof:-
// Consider S=(0,1) - a point of order 3 - as the random contribution in the 
// "normal" Tate algorithm i.e. Q <- (Q+S)-(S). Note that the coordinates of P
// are both in Fp. Under these circumstances the contribution of S to the 
// numerator and denominator of the cumulative multiplicative total will always
// be in Fp. All such contributions will be "wiped out" by the final raising 
// to the power of (p-1). Therefore S can be ommitted altogether, and all 
// calculations involving it removed.
//
// Furthermore the calculation will not fail. The numerator and denominator 
// will never evaluate to 0, as a consequence of linear independence. The point 
// Q cannot be both on the curve and on a line joining two point in the 
// calculation of r.P
//
// P & Q(x,y) are both points of order q. 
// Note that P is a point on the curve over Fp, Q(x,y) a point on the 
// quadratic extension field Fp^2
//

BOOL fast_tate_pairing(ECn& P,ZZn2& Qx,ZZn& Qy,Big& q,ZZn2& res)
{ 
    int i;
    Big p;
    ECn A;
    ZZn2 ha,had;

#ifndef SIMPLE
    ECn P2,t[11];
    ZZn2 hc,hcd,z2n,z2d,zn[11],zd[11];
    Big q3;
#endif

    ha=had=1; 

#ifdef SIMPLE

// q.P = 2^17*(2^142.P +P) + P

    A=P;    // reset A
    for (i=0;i<142;i++)
    {
        ha*=ha; had*=had;          
        g(A,A,Qx,Qy,ha,had,ADD,FALSE);      // 16 ZZn muls + 1 inverse 
        if (ha==0 || had==0) return FALSE;
    }                                       // 30 ZZn muls (Projective)
    g(A,P,Qx,Qy,ha,had,ADD,FALSE);          // 11 ZZn muls + 1 inverse 
    if (ha==0 || had==0) return FALSE;
    for (i=0;i<17;i++)                      // 34 ZZn muls (Projective)
    {
        ha*=ha; had*=had;          
        g(A,A,Qx,Qy,ha,had,ADD,FALSE);      // 16 ZZn muls + 1 inverse 
        if (ha==0 || had==0) return FALSE;
    }                                       // 30 ZZn muls (Projective)
    g(A,P,Qx,Qy,ha,had,ADD,FALSE);          // 11 ZZn muls + 1 inverse 
    if (ha==0 || had==0) return FALSE;
                                            // 34 ZZn muls (Projective)
#else

    q3=q*3;
    zn[0]=zd[0]=1;
    t[0]=P2=A=P;
    g(P2,P2,Qx,Qy,z2n,z2d,ADD,TRUE);     // P2=P+P
//
// Build NAF windowing table
//
    for (i=1;i<11;i++)
    {                                    // 17 ZZn muls + 1 inverse (Affine)
        g(A,P2,Qx,Qy,hc,hcd,ADD,TRUE);   // 40 ZZn muls (Projective)    
        t[i]=A;                          // precalculate t[i] = (2i+1).P
        zn[i]=z2n*zn[i-1]*hc;
        zd[i]=z2d*zd[i-1]*hcd;
    }

    A=P;    // reset A

// Left to right method
    nb=bits(q3);
    for (i=nb-2;i>=1;i-=(nbw+nzs))
    {
        n=naf_window(q,q3,i,&nbw,&nzs);  // standard MIRACL NAF windowing

        for (j=0;j<nbw;j++)
        {
            ha*=ha; had*=had;          
            g(A,A,Qx,Qy,ha,had,ADD,FALSE);      // 16 ZZn muls + 1 inverse 
        }                                       // 30 ZZn muls (Projective) 
        if (n>0)
        {
            ha*= zn[n/2]; had*=zd[n/2];    
            g(A,t[n/2],Qx,Qy,ha,had,ADD,FALSE); // 17 ZZn muls + 1 inverse 
        }                                       // 40 ZZn muls (Projective)
        if (n<0)
        {
            n=(-n);
            ha*=zd[n/2]; had*=zn[n/2];
            g(A,t[n/2],Qx,Qy,ha,had,SUB,FALSE); // 17 ZZn muls + 1 inverse
        }                                       // 40 ZZn muls (Projective)
        for (j=0;j<nzs;j++) 
        {
            ha*=ha; had*=had;
            g(A,A,Qx,Qy,ha,had,ADD,FALSE);      // 16 ZZn muls + 1 inversion
        }                                       // 30 ZZn muls (Projective) 
        if (ha==0 || had==0) return FALSE;
    }

#endif

    if (!A.iszero()) return FALSE;

    res=(ha/had);

    p=get_modulus();         // get p
    res= pow(res,(p+1)/q);   // raise to power of (p^2-1)/q
    res=conj(res)/res;
    if (res.isunity()) return FALSE;
    return TRUE;   
}

//
// ecap(.) function
//

BOOL ecap(ECn& P,ECn& Q,Big& order,ZZn2& cube,ZZn2& res)
{
    ZZn2 Qx;
    ZZn  Qy,iy;
    Big xx,yy;

    Q.get(xx,yy);
    Qx=(ZZn)xx*cube;
    Qy=(ZZn)yy;

    iy=(ZZn)1/(Qy+1);
    Qx=-2*Qx*iy;
    Qy=(Qy-3)*iy;        // Q+=(0,1)

    if (fast_tate_pairing(P,Qx,Qy,order,res)) return TRUE;
    return FALSE;
}

//
// Hash functions
// 

Big H1(char *string)
{ // Hash a zero-terminated string to a number < modulus
    Big h,p;
    char s[HASH_LEN];
    int i,j; 
    sha sh;

    shs_init(&sh);

    for (i=0;;i++)
    {
        if (string[i]==0) break;
        shs_process(&sh,string[i]);
    }
    shs_hash(&sh,s);
    p=get_modulus();
    h=1; j=0; i=1;
    forever
    {
        h*=256; 
        if (j==HASH_LEN)  {h+=i++; j=0;}
        else         h+=s[j++];
        if (h>=p) break;
    }
    h%=p;
    return h;
}

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
   
//
// Given y, get x=(y^2-1)^(1/3) mod p (from curve equation)
//

Big getx(Big y)
{
    Big p=get_modulus();
    Big t=modmult(y+1,y-1,p);   // avoids overflow
    return pow(t,(2*p-1)/3,p);
}
 
//
// MapToPoint
//

ECn map_to_point(char *ID)
{
    ECn Q;
    Big x0,y0=H1(ID);
 
    x0=getx(y0);

    Q.set(x0,y0);

    return Q;
}

void strip(char *name)
{ /* strip off filename extension */
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
    miracl *mip=mirsys(18,0);   // thread-safe ready. (36,0) for 1024 bit p
    ifstream common("common.ibe");
    ifstream plaintext;
    ofstream key_file,ciphertext;
    ECn U,P,Ppub,Qid,infinity;
    ZZn2 gid,cube,w;
    char key[HASH_LEN],pad[HASH_LEN],rho[HASH_LEN],V[HASH_LEN],W[HASH_LEN];
    char ifname[100],ofname[100],ch,iv[16];
    Big p,q,r,x,y,cof;
    int i,bits;
    long seed;
    aes a;

    cout << "Enter 9 digit random number seed  = ";
    cin >> seed;
    irand(seed);

// ENCRYPT

    common >> bits;
    mip->IOBASE=16;
    common >> p >> q;

    cof=(p+1)/q;

    common >> x >> y;
    EBrick B(x,y,(Big)0,(Big)1,p,QBITS);   // precomputation based on fixed P

#ifdef AFFINE
    ecurve(0,1,p,MR_AFFINE);
#endif
#ifdef PROJECTIVE
    ecurve(0,1,p,MR_PROJECTIVE);
#endif

    P.set(x,y);

    common >> x >> y;
    Ppub.set(x,y);

    common >> x >> y;
    cube.set(x,y);

    char id[1000];
    cout << "Enter your correspondents email address (lower case)" << endl;
    cin.get();
    cin.getline(id,1000);

    mip->IOBASE=10;
    Qid=map_to_point(id);

// This can be done before we know the message to encrypt

    if (!ecap(Ppub,Qid,q,cube,gid))      // ** swap argument order
    {                                    // Qid must be second
        cout << "Bad Parameters" << endl;
        exit(0);
    } 

//
// prepare to encrypt file with random session key
//

    for (i=0;i<HASH_LEN;i++) key[i]=(char)brand();
    for (i=0;i<16;i++) iv[i]=i; // set CFB IV
    aes_init(&a,MR_CFB1,16,key,iv);
    
// figure out where input is coming from

    cout << "Text file to be encoded = " ;
    cin >> ifname;

   /* set up input file */
    strcpy(ofname,ifname);
    strip(ofname);
    strcat(ofname,".ibe");
    plaintext.open(ifname,ios::in); 
    if (!plaintext)
    {
        cout << "Unable to open file " << ifname << "\n";
        return 0;
    }
    cout << "encoding message\n";
    ciphertext.open(ofname,ios::binary|ios::out);

// now encrypt the plaintext file

    forever
    { // encrypt input ..
        plaintext.get(ch);
        if (plaintext.eof()) break;
        aes_encrypt(&a,&ch);
        ciphertext << ch;
    }

    aes_end(&a);

//
// Now IBE encrypt the session key
//

    for (i=0;i<HASH_LEN;i++) rho[i]=(char)brand();

    r=H3(rho,key);

    B.mul(r,x,y);       // U=r*P
    U.set(x,y);

    w=pow(gid,r);         
    H2(w,pad);
    
    for (i=0;i<HASH_LEN;i++) 
    {
        V[i]=rho[i]^pad[i];
        pad[i]=0;
    }
    H4(rho,rho);
    for (i=0;i<HASH_LEN;i++) 
    {
        W[i]=key[i]^rho[i];
        rho[i]=0;
    }

    strip(ofname);
    strcat(ofname,".key");
    mip->IOBASE=16;
    key_file.open(ofname);
    U.get(x,y);
    key_file << y << endl;
    x=from_binary(20,V);      // output bit strings in handy Big format
    key_file << x << endl;
    x=from_binary(20,W);
    key_file << x << endl;

    return 0;
}


