//
// Agrawal, Kayal & Saxena Prime Prover (Conjecture 4)
//
// cl /O2 /GX tp.cpp polymod.cpp poly.cpp monty.cpp big.cpp ms32.lib
//
// Note neat way of assigning polynomials via the dummy type Variable
//

#include <iostream>
#include <big.h>
#include "poly.h"
#include "polymod.h"

using namespace std;

Miracl precision=100;

int main(int argc,char **argv)
{
    Big n;
    int r;

    if (argc!=2)
    {
        cout << "tp <number>" << endl;
        return 0;
    }

    n=argv[1];
    if (n==2)
    {
        cout << "PRIME" << endl;
        return 0;
    }
    if (small_factors(n))
    {
        cout << "COMPOSITE" << endl;
        return 0;
    }

    if (perfect_power(n))
    {
        cout << "COMPOSITE" << endl;
        return 0;
    }

    for (r=2;;r++)
        if ((n*n-1)%r!=0) break;

    modulo(n);

    Variable x;
    Poly M=pow(x,r)-1;  // M=x^r-1

    setmod(M);

    PolyMod lhs,rhs;

    lhs=x-1;            // left-hand side
    lhs=pow(lhs,n);     // (x-1)^n mod M

    rhs=x;              // right-hand side
    rhs=pow(rhs,n)-1;   // x^n-1   mod M

    if (lhs==rhs) cout << "PRIME" << endl;
    else          cout << "COMPOSITE" << endl;

    return 0;
}

