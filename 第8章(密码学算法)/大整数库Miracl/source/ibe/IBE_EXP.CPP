/*
   Boneh & Franklin's Identity Based Encryption
  
   Encryption phase
  
   Generates a random AES session key, and uses it to encrypt a file.
   Outputs ciphertext <filename>.ibe.

   The session key is IBE encrypted, and written to <filename>.key

   NOTE: Uses Tate Pairing only
   NOTE: New fast Tate pairing algorithm
   NOTE: Assumes SIMPLE fixed group order q = 2^159+2^17+1

   Pre-computation version.

   In a busy IBE email client this might be useful for sending emails
   to multiple recipients.
 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <elliptic.h>
#include <monty.h>
#include <ebrick.h>
#include "ecn2.h"
#include "zzn2.h"

using namespace std;

#define PBITS 512
#define QBITS 160

// Using SHA-1 as basic hash algorithm

#define HASH_LEN 20

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

//
// Add A=A+B  (or A=A+A)
// Bump up num and denom
//
// On first pass through precomp=FALSE, and so all points
// and slopes are "recorded" in store[]
//
// On subsequent passes these values ( total < 500 ) are "played back"
//

void g(ECn& A,ECn& B,ZZn& Qx,ZZn2& Qy,ZZn2& num,BOOL precomp,ZZn* store,int& ptr)
{
    ZZn  lam,x,y,m,nx;
    ZZn2 u;
    big pointer;
    if (num.iszero()) return;

    if (!precomp)
    { // Store line start point and slope.
      // Evaluate line from A, and then evaluate vertical through destination
        extract(A,x,y); 
        pointer=A.add(B);
        if (pointer==NULL) {num=0; return;} 
        lam=pointer;

        store[ptr++]=x; store[ptr++]=y; store[ptr++]=lam; 
        if (A.iszero()) return;   
// line
        m=Qx; u=Qy;
        m-=x; m*=lam;            // 1 ZZn muls
        u-=y; u-=m;
    }
    else
    { // extract precalculated values from the store.... - nx is a peek ahead
        x=store[ptr++]; y=store[ptr++]; lam=store[ptr++]; nx=store[ptr];
        if (nx.iszero()) return;

        m=Qx; u=Qy;
        m-=x; m*=lam;              // 1 ZZn muls
        u-=y; u-=m;
    }
 
    num*=u;                        // 3 ZZn muls  
}

//
// Tate Pairing 
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
// P is of order q and Q(x,y) has an order a multiple of q.. 
// Note that P is a point on the curve over Fp, Q(x,y) a point on the 
// quadratic extension field Fp^2
//
// When P is fixed, precomputation helps. Note that each time we are 
// calculating q.P where q and P are fixed, and the result O is known. So 
// store all points and slopes for re-use the next time. Set precomp=FALSE first
// time, and then precomp=TRUE in subsequent calls. Initialise store to hold 
// precomputed ZZn's (about 500 of them).   
//

BOOL fast_tate_pairing(ECn& P,ZZn& Qx,ZZn2& Qy,Big& q,BOOL precomp,ZZn* store,ZZn2& res)
{ 
    int i,ptr=0;
    Big p;
    ECn A;

    if (!precomp) get_mip()->coord=MR_AFFINE; // precompute using AFFINE 
                                              // coordinates
    res=1; 

// q.P = 2^17*(2^142.P +P) + P

    A=P; 
    for (i=0;i<142;i++)
    {
        res*=res;          
        g(A,A,Qx,Qy,res,precomp,store,ptr);
    }                                   // 6 ZZn muls after first
    g(A,P,Qx,Qy,res,precomp,store,ptr);

    for (i=0;i<17;i++)                      
    {
        res*=res;          
        g(A,A,Qx,Qy,res,precomp,store,ptr);
    } 
    g(A,P,Qx,Qy,res,precomp,store,ptr);

    if (res.iszero()) return FALSE;

    if (!precomp) 
    {
        if (!A.iszero()) return FALSE;
        get_mip()->coord=MR_PROJECTIVE; // reset 
    }

    p=get_modulus();         // get p
    res= pow(res,(p+1)/q);   // raise to power of (p^2-1)/q
    res=conj(res)/res;
    if (res.isunity()) return FALSE;

    return TRUE;         
}

//
// ecap(.) function
//

BOOL ecap(ECn& P,ECn& Q,Big& order,BOOL precomp,ZZn *store,ZZn2& res)
{
    ZZn  Qx;
    ZZn2 Qy;
    Big xx,yy;
    BOOL Ok;
    Q.get(xx,yy);
    Qx=-xx;
    Qy.set((ZZn)0,yy);

    Ok=fast_tate_pairing(P,Qx,Qy,order,precomp,store,res);

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
// MapToPoint
//

ECn map_to_point(char *ID)
{
    ECn Q;
    Big x0=H1(ID);
 
    while (!Q.set(x0,x0)) x0=x0+1;

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
    ifstream common("commonx.ibe");
    ifstream plaintext;
    ofstream key_file,ciphertext;
    ECn U,P,Ppub,Qid,infinity;
    ZZn2 gid,w;
    ZZn *store;
    char key[HASH_LEN],pad[HASH_LEN],rho[HASH_LEN],V[HASH_LEN],W[HASH_LEN];
    char ifname[100],ofname[100],ch,iv[16];
    Big p,q,r,x,y,cof;
    int i,bits;
    long seed;
    aes a;
    BOOL Ok,precomp=FALSE;

    cout << "Enter 9 digit random number seed  = ";
    cin >> seed;
    irand(seed);

// ENCRYPT

    common >> bits;
    mip->IOBASE=16;
    common >> p >> q;

    cof=(p+1)/q;

    common >> x >> y;
    EBrick B(x,y,(Big)1,(Big)0,p,QBITS);   // precomputation based on P

    ecurve(1,0,p,MR_PROJECTIVE);

    P.set(x,y);

    common >> x >> y;
    Ppub.set(x,y);

    store=new ZZn[500];

    char id[1000];
    cout << "Enter your correspondents email address (lower case)" << endl;
    cin.get();
    cin.getline(id,1000);

    mip->IOBASE=10;
    Qid=map_to_point(id);

// This can be done before we know the message to encrypt

    for (int times=0;times<2;times++)
    {

        Ok=ecap(Ppub,Qid,q,precomp,store,gid); 
        if (!Ok)
        {  /* Ppub is not of order q ! */
            cout << "Bad Parameters" << endl;
            exit(0);
        } 

// Do it again to demonstrate that precomputation has worked

        precomp=TRUE;

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
        ciphertext.close();
        plaintext.clear();
        plaintext.close();
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

        key_file << x << endl;
        key_file << y << endl;
        x=from_binary(20,V);      // output bit strings in handy Big format
        key_file << x << endl;
        x=from_binary(20,W);
        key_file << x << endl;
        key_file.close();
    }

    return 0;
}


