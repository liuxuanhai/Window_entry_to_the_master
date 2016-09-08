/*
 *    MIRACL C++ Header file gf2m.h
 *
 *    AUTHOR  : M.Scott
 *    
 *    PURPOSE : Definition of class GF2m (Arithmetic in the field GF(2^m)
 *
 *    NOTE:   : The field basis is set dynamically via the modulo() routine.
 *              Must be used with big.h and big.cpp
 *
 *    Copyright (c) 2000 Shamus Software Ltd.
 */

#ifndef GF2M_H
#define GF2M_H

#include <big.h>

class GF2m
{
    Big fn;
public:
    GF2m()   { }
    GF2m(int i)        {if (i==0) fn=0; else reduce2((Big)i,fn);}
    GF2m(long lg)      {if (lg==0L) fn=0; else reduce2((Big)lg,fn);}
    GF2m(const Big& b) {reduce2(b,fn); }   /* Big -> GF2m */
    GF2m(big& b)       {copy(b,fn.getbig());}
    GF2m(const GF2m& b) {fn=b.fn;}
    GF2m(char *s)      {reduce2((Big)s,fn);}
   
    GF2m& operator=(int i)   {if (i==0) fn=0; else reduce2((Big)i,fn); return *this; }
    GF2m& operator=(long lg) 
                    {if (lg==0L) fn=0; else reduce2((Big)lg,fn); return *this;}

    GF2m& operator=(const Big& b) { reduce2(b,fn); return *this; }
    GF2m& operator=(const GF2m b) {fn=b.fn; return *this;}
    GF2m& operator=(char *s)      {reduce2((Big)s,fn); return *this;}
    GF2m& operator=(big b) {copy(b,fn.getbig()); return *this;}


    GF2m& operator++() {incr2(fn,1,fn); return *this; }
    GF2m& operator+=(int i) {incr2(fn,i,fn); return *this; }
    GF2m& operator+=(const GF2m& b) {add2(fn,b.fn,fn); return *this;}
    GF2m& operator*=(const GF2m& b) {mul2(fn,b.fn,fn); return *this;}

    BOOL iszero() const;
    BOOL isone() const;
    operator Big() {return fn;}   /* GF2m -> Big */
    friend big getbig(GF2m& z) {return z.fn.getbig();}
    friend int trace(GF2m & z) {return trace2(z.fn.getbig());}


    GF2m& operator/=(const GF2m& b) {fn=div2(fn,b.fn); return *this;}
    
    friend GF2m operator+(const GF2m&,const GF2m&);
    friend GF2m operator+(const GF2m&,int);
    friend GF2m operator*(const GF2m&,const GF2m&);
    friend GF2m operator/(const GF2m&,const GF2m&);
    
    friend BOOL operator==(const GF2m& b1,const GF2m& b2)
    { if (b1.fn==b2.fn) return TRUE; else return FALSE;}
    friend BOOL operator!=(const GF2m& b1,const GF2m& b2)
    { if (b1.fn!=b2.fn) return TRUE; else return FALSE;}
     
    friend GF2m pow(const GF2m&,int);
    friend GF2m sqrt(const GF2m&);
        
    ~GF2m() { }
};


#endif  

