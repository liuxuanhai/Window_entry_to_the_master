/*
   Boneh & Franklin's Identity Based Encryption  - y2=x^3+x version

   Extract Phase

   After this program has run the file privatex.ibe contains

   <Private point Did - y coordinate>

 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <elliptic.h>
#include <monty.h>

using namespace std;

// Using SHA-1 as basic hash algorithm

#define HASH_LEN 20

//
// Hash function
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

int main()
{
    miracl *mip=mirsys(18,0);     // thread-safe ready.  (36,0) for 1024 bit p
    ifstream common("commonx.ibe");
    ifstream master("masterx.ibe");
    ofstream private_key("privatex.ibe");
    ECn Qid,Did;
    Big p,q,cof,s,x,y;
    int bits;

    common >> bits;
    mip->IOBASE=16;
    common >> p >> q;
    master >> s;
    mip->IOBASE=10;

    ecurve(1,0,p,MR_PROJECTIVE);
    cof=(p+1)/q;

// EXTRACT

    char id[1000];

    cout << "Enter your email address (lower case)" << endl;
    cin.getline(id,1000);

    Qid=map_to_point(id);

    Did=s*Qid;

    cout << "Private key= " << Did << endl;

    Did.get(x,y);
    mip->IOBASE=16;

    private_key << x << endl;
    private_key << y << endl;

    return 0;
}

