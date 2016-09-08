/*
 *    MIRACL  C++ Header file monty.h
 *
 *    AUTHOR  : M. Scott
 *  
 *    PURPOSE : Definition of class ZZn  (Arithmetic mod n), using 
 *              Montgomery's Method for modular multiplication
 *    NOTE    : Must be used in conjunction with big.cpp and monty.cpp
 *              The modulus n is always set dynamically (via the modulo() 
 *              routine) - so beware the pitfalls implicit in declaring
 *              static or global ZZn's (which are initialised before n is 
 *              set!). Uninitialised data is OK 
 *                
 *    Copyright (c) 1988-2001 Shamus Software Ltd.
 */

#ifndef MONTY_H
#define MONTY_H

#include <big.h>

class ZZn 
{ 
    Big fn;
public:
    ZZn()       {  } 
    ZZn(int i)  { if (i==0) fn=0; else fn=nres((Big)i); }
    ZZn(long lg){ if (lg==0L) fn=0; else fn=nres((Big)lg); }
    ZZn(const Big& b) { fn=nres(b); }   /* Big -> ZZn */
    ZZn(big& b)        {copy(b,fn.getbig());}
    ZZn(const ZZn& b) { fn=b.fn; }
    ZZn(char* s){ fn=nres((Big)s); }

    ZZn& operator=(int i) {if (i==0) fn=0; else fn=nres((Big)i); return *this;}
    ZZn& operator=(long lg)
                      {if (lg==0L) fn=0; else fn=nres((Big)lg); return *this;}
    ZZn& operator=(const ZZn& b){fn=b.fn; return *this;}
    ZZn& operator=(char* s){fn=nres((Big)s); return *this;}
    ZZn& operator=(big b) {copy(b,fn.getbig()); return *this; }

/* Use fast in-line code */

    ZZn& operator++() 
        {nres_modadd(fn,nres((Big)1),fn);return *this;}
    ZZn& operator--() 
        {nres_modsub(fn,nres((Big)1),fn);return *this;}
    ZZn& operator+=(int i) 
        {nres_modadd(fn,nres((Big)i),fn);return *this;}
    ZZn& operator-=(int i) 
        {nres_modsub(fn,nres((Big)i),fn); return *this;}
    ZZn& operator+=(const ZZn& b) 
        {nres_modadd(fn,b.fn,fn); return *this;}
    ZZn& operator-=(const ZZn& b) 
        {nres_modsub(fn,b.fn,fn); return *this;}
    ZZn& operator*=(const ZZn& b) 
        {nres_modmult(fn,b.fn,fn); return *this;}
    ZZn& operator*=(int i) 
        {nres_premult(fn,i,fn); return *this;}

    BOOL iszero() const;
    BOOL isminusone() const;

    operator Big() {return redc(fn);}   /* ZZn -> Big */
    friend big getbig(ZZn& z) {return z.fn.getbig();}

    ZZn& operator/=(const ZZn& b) {nres_moddiv(fn,b.fn,fn); return *this;}
    ZZn& operator/=(int i) {nres_moddiv(fn,nres((Big)i),fn);return *this;}

    friend ZZn operator-(const ZZn&);
    friend ZZn operator+(const ZZn&,int);
    friend ZZn operator+(int, const ZZn&);
    friend ZZn operator+(const ZZn&, const ZZn&);

    friend ZZn operator-(const ZZn&, int);
    friend ZZn operator-(int, const ZZn&);
    friend ZZn operator-(const ZZn&, const ZZn&);

    friend ZZn operator*(const ZZn&,int);
    friend ZZn operator*(int, const ZZn&);
    friend ZZn operator*(const ZZn&, const ZZn&);

    friend ZZn operator/(const ZZn&, int);
    friend ZZn operator/(int, const ZZn&);
    friend ZZn operator/(const ZZn&, const ZZn&);

    friend BOOL operator==(const ZZn& b1,const ZZn& b2)
    { if (b1.fn==b2.fn) return TRUE; else return FALSE;}
    friend BOOL operator!=(const ZZn& b1,const ZZn& b2)
    { if (b1.fn!=b2.fn) return TRUE; else return FALSE;}

    friend ZZn  pow( const ZZn&, const Big&);
    friend ZZn  pow( const ZZn&,int);
    friend ZZn  pow( const ZZn&, const Big&, const ZZn&, const Big&);
    friend ZZn  pow( int,ZZn *,Big *);    

    friend ZZn  randn(void);      // random number < modulus
    friend BOOL qr(ZZn&);   // test for quadratic residue
    friend BOOL qnr(ZZn&);  // test for quadratic non-residue
    friend ZZn getA(void);        // get A parameter of elliptic curve
    friend ZZn getB(void);        // get B parameter of elliptic curve

    friend ZZn  sqrt(const ZZn&);          // only works if modulus is prime
    friend ZZn  luc( const ZZn&, const Big&, ZZn* b3=NULL);
    ~ZZn() { }
};

#endif

