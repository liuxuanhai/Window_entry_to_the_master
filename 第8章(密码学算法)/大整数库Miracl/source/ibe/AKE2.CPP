/*
   Scott's AKE Client/Server testbed

   See http://www.compapp.dcu.ie/research/CA_Working_Papers/wp02.html#0202

   Compile as 
   cl /O2 /GX /DBIGS=18 ake2.cpp zzn2.cpp ecn2.cpp big.cpp monty.cpp elliptic.cpp miracl.lib
   using COMBA build

   Requires file k2.ecs which contains details of non-supersingular 
   elliptic curve, with order divisible by q=2^159+2^17+1, and security 
   multiplier k=2. The prime p is 512 bits

   NOTE: Key exchange bandwidth could be reduced by halve using ideas from 
         "Doing more with Fewer Bits", Brouwer, Pellikaan & Verheul, Asiacrypt 
         '99

*/

#include <iostream>
#include <fstream>
#include <elliptic.h>
#include <monty.h>
#include <ctime>
#include "ecn2.h"
#include "zzn2.h"

using namespace std;

Miracl precision(18,0); 

// Using SHA-512 as basic hash algorithm

#define HASH_LEN 64

//
// Define one or the other of these
//
// Which is faster depends on the I/M ratio - See imratio.c
// Roughly if I/M ratio > 16 use PROJECTIVE, otherwise use AFFINE
//

// #define AFFINE
#define PROJECTIVE

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
    m-=x; m*=slope;  
    w-=y; w-=m;
#endif
#ifdef PROJECTIVE
    extract(A,x,y,z);
    x*=z; t=z; z*=z; z*=t;          
    m*=z; m-=x;    
    w*=z; w-=y; 
    extract(C,x,y,z);
    w*=z; m*=slope; w-=m;                     
#endif
    return w;
}

//
// Add A=A+B  (or A=A+A) 
// Bump up num
//

void g(ECn& A,ECn& B,ZZn& Qx,ZZn2& Qy,ZZn2& num)
{
    ZZn  lam;
    ZZn2 u;
    ECn P=A;
    big ptr;

// Evaluate line from A - lam is line slope

    ptr=A.add(B);
    if (ptr==NULL)    { num.clear(); return; }
    lam=ptr;

    if (A.iszero())   return; 

    u=line(P,A,lam,Qx,Qy);

    num*=u;    
}

//
// Tate Pairing - note denominator elimination has been applied
//
// P is a point of order q. Q(x,y) is a point of order q. 
// Note that P is a point on the curve over Fp, Q(x,y) a point on the 
// extension field Fp^2
//

BOOL fast_tate_pairing(ECn& P,ZZn& Qx,ZZn2& Qy,Big& q,ZZn2& res)
{ 
    int i,j;
    Big p;
    ECn A;

    res=1;  

// Note that q is fixed - q.P=2^17*(2^142.P + P) + P

    A=P;    // reset A
    for (i=0;i<142;i++)
    {
        res*=res;           
        g(A,A,Qx,Qy,res); 
    }
    g(A,P,Qx,Qy,res);
    for (i=0;i<17;i++)
    {
        res*=res;           
        g(A,A,Qx,Qy,res); 
    }
    g(A,P,Qx,Qy,res);

    if (!A.iszero() || res.iszero()) return FALSE;
    p=get_modulus();
    res=pow(res,(p+1)/q);
    res=conj(res)/res;     // raise to power of (p^2-1)/q

    if (res.isunity()) return FALSE;
    return TRUE;            
}

//
// co-gap ecap(.) function
//

BOOL ecap(ECn& P,ECn2& Q,Big& order,ZZn2& res)
{
    BOOL Ok;
    ZZn2 Qx,Qy;
    ZZn Qxx;
    Big t;

    Q.get(Qx,Qy);
    Qx.get(t);
    Qxx=t;
    Ok=fast_tate_pairing(P,Qxx,Qy,order,res);

    return Ok;
}

//
// Hash functions
// 

Big H1(char *string)
{ // Hash a zero-terminated string to a number < modulus
    Big h,p;
    char s[HASH_LEN];
    int i,j; 
    sha512 sh;

    shs512_init(&sh);

    for (i=0;;i++)
    {
        if (string[i]==0) break;
        shs512_process(&sh,string[i]);
    }
    shs512_hash(&sh,s);
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

Big H2(ZZn2 x)
{ // Hash an Fp2 to a big number
    sha sh;
    Big a,b,h,p;
    char s[20];
    int i,j,m;

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
    h=from_binary(20,s);
    return h;
}

// Hash and map a Server Identity to a curve point E_(Fp2)

ECn2 hash_and_map2(char *ID)
{
    ECn T;
    ECn2 S;
    ZZn2 X,Y;
    Big p,x,a=H1(ID);
    p=get_modulus();

    while (T.set(x)) a+=1;  // Make sure its not on E(F_p)
    X.set(a);               // Map & Hash "Identity" to curve point
    S.set(X);               // This will never fail

    return S;
}     

// Hash and map a Client Identity to a curve point E_(Fp)

ECn hash_and_map(char *ID,Big cof)
{
    ECn Q;
    Big x0=H1(ID);

    while (!Q.set(x0)) x0+=1;
    Q*=cof;
    return Q;
}

/* Note that if #E(Fp)  = p+1-t
           then #E(Fp2) = (p+1-t)(p+1+t) (a multiple of #E(Fp))
           (Weil's Theorem)
 */

int main()
{
    ifstream common("k2.ecs");      // elliptic curve parameters
    miracl* mip=&precision;
    ECn Alice,Bob,sA,sB;
    ECn2 B2,Server,sS;
    ZZn2 X,Y,res,sp,ap,bp;
    Big t,r,a,b,s,ss,p,q,x,y,B,cf,cf2;
    int i,bits,A;
    long seed;

    common >> bits;
    mip->IOBASE=16;
    common >> p;
    common >> A;
    common >> B >> r;

    t=p+1-r;

    q=pow((Big)2,159)+pow((Big)2,17)+1;
    cf= (p+1-t)/q;        // q divides p+1 (for k=2 condition)
    cf2=(p+1+t)/q;      // and therefore also divides t (as it divides r)

// this co-factor is in fact not needed....

    time(&seed);
    irand(seed);

#ifdef AFFINE
    ecurve(A,B,p,MR_AFFINE);
#endif
#ifdef PROJECTIVE
    ecurve(A,B,p,MR_PROJECTIVE);
#endif

    mip->IOBASE=16;

// hash Identities to curve point

    ss=rand(q);    // TA's super-secret 

    cout << "Mapping Server ID to point" << endl;

    Server=hash_and_map2("Server");

    cout << "Mapping Alice & Bob ID's to points" << endl;

    Alice=hash_and_map("Alice",cf);
    Bob=  hash_and_map("Robert",cf);

// Alice, Bob are points of order q
// Server does not need to be (its order is a multiple of q)

    cout << "Alice, Bob and the Server visit Trusted Authority" << endl;

    sS=ss*Server; 
    sA=ss*Alice;
    sB=ss*Bob;

    cout << "Alice and Server Key exchange" << endl;

    a=rand(q);   // Alice's random number
    s=rand(q);   // Server's random number

    if (!ecap(sA,Server,q,res)) cout << "Trouble" << endl; 
    ap=pow(res,a);

    if (!ecap(Alice,sS,q,res))  cout << "Trouble" << endl;
    sp=pow(res,s);

    cout << "Alice  Key= " << H2(pow(sp,a)) << endl;
    cout << "Server Key= " << H2(pow(ap,s)) << endl;

    cout << "Bob and Server Key exchange" << endl;

    b=rand(q);   // Bob's random number
    s=rand(q);   // Server's random number

    if (!ecap(sB,Server,q,res)) cout << "Trouble" << endl;
    bp=pow(res,b);

    if (!ecap(Bob,sS,q,res)) cout << "Trouble" << endl;
    sp=pow(res,s);

    cout << "Bob's  Key= " << H2(pow(sp,b)) << endl;
    cout << "Server Key= " << H2(pow(bp,s)) << endl;

    cout << "Alice and Bob's attempted Key exchange" << endl;

    Bob.get(x,y);
    X.set(x); Y.set(y);
    B2.set(X,Y);

    ecap(Alice,B2,q,res);

    cout << "But Tate Pairing evaluates as.... :( " << res << endl;

    return 0;
}

