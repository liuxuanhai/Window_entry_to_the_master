/*
 *    MIRACL  C++ Header file ZZn6.h
 *
 *    AUTHOR  : M. Scott
 *
 *    NOTE:   : Must be used in conjunction with big.cpp and monty.cpp
 *
 *    PURPOSE : Definition of class ZZn6  (Arithmetic over n^6)
 *
 * WARNING: This class has been cobbled together for a specific use with
 * the MIRACL library. It is not complete, and may not work in other 
 * applications
 *
 *
 *    Copyright (c) 2001 Shamus Software Ltd.
 */

#ifndef ZZN6_H
#define ZZN6_H

#include <monty.h>

extern void init_zzn6(Big);

class ZZn6
{
    ZZn a[6];
public:
    ZZn6()   {}
    ZZn6(int w) {a[0]=(ZZn)w; a[1]=a[2]=a[3]=a[4]=a[5]=0;}
    ZZn6(const ZZn6& w) {a[0]=w.a[0]; a[1]=w.a[1]; a[2]=w.a[2];
                         a[3]=w.a[3]; a[4]=w.a[4]; a[5]=w.a[5];}

    ZZn6(const Big &x)  {a[0]=(ZZn)x; a[1]=a[2]=a[3]=a[4]=a[5]=0;}
    
    void set(const Big *x) {a[0]=(ZZn)x[0]; a[1]=(ZZn)x[1];a[2]=(ZZn)x[2]; 
                            a[3]=(ZZn)x[3]; a[4]=(ZZn)x[4];a[5]=(ZZn)x[5];}

    void set(const Big &x) {a[0]=(ZZn)x; a[1]=a[2]=a[3]=a[4]=a[5]=0;}

    void get(Big *) ;
    void get(Big &) ;
    
    void clear() {a[0]=a[1]=a[2]=a[3]=a[4]=a[5]=0;}
    
    BOOL iszero()  const {if (a[0]==0 && a[1]==0 && a[2]==0 
                           && a[3]==0 && a[4]==0 && a[5]==0) return TRUE; return FALSE; }
    BOOL isunity() const {if (a[0]==(ZZn)1 && a[1]==0 && a[2]==0 
                           && a[3]==0 && a[4]==0 && a[5]==0) return TRUE; return FALSE; }
    BOOL isminusone() const {if (a[0]==-1 && a[1]==0 && a[2]==0 
                           && a[3]==0 && a[4]==0 && a[5]==0) return TRUE; return FALSE; }

    ZZn6& powq();
    ZZn6& operator=(int i) {a[0]=i; a[1]=a[2]=a[3]=a[4]=a[5]=0; return *this;}
    ZZn6& operator=(const ZZn& x)  {a[0]=x; a[1]=a[2]=a[3]=a[4]=a[5]=0; return *this; }
    ZZn6& operator=(const ZZn6& x) {a[0]=x.a[0]; a[1]=x.a[1]; a[2]=x.a[2];  
                                    a[3]=x.a[3]; a[4]=x.a[4]; a[5]=x.a[5]; return *this; }
    ZZn6& operator+=(const ZZn& x) {a[0]+=x; return *this; }
    ZZn6& operator+=(const ZZn6& x) {a[0]+=x.a[0]; a[1]+=x.a[1]; a[2]+=x.a[2];
                                     a[3]+=x.a[3]; a[4]+=x.a[4]; a[5]+=x.a[5]; 
                                     return *this; }
    ZZn6& operator-=(const ZZn& x)  {a[0]-=x; return *this; }
    ZZn6& operator-=(const ZZn6& x) {a[0]-=x.a[0]; a[1]-=x.a[1]; a[2]-=x.a[2];
                                     a[3]-=x.a[3]; a[4]-=x.a[4]; a[5]-=x.a[5]; 
                                     return *this; }
    ZZn6& operator*=(const ZZn6&); 
    ZZn6& operator*=(const ZZn& x) { a[0]*=x; a[1]*=x; a[2]*=x;
                                     a[3]*=x; a[4]*=x; a[5]*=x;  return *this;}
    ZZn6& operator*=(int x) { a[0]*=x; a[1]*=x; a[2]*=x;
                              a[3]*=x; a[4]*=x; a[5]*=x;  return *this;}
    ZZn6& operator/=(const ZZn6&); 
    ZZn6& operator/=(const ZZn& x) { a[0]/=x; a[1]/=x; a[2]/=x;
                                     a[3]/=x; a[4]/=x; a[5]/=x; return *this; }
    ZZn6& invert(void);

    friend int  degree(const ZZn6&);
    friend ZZn6 operator+(const ZZn6&,const ZZn6&);
    friend ZZn6 operator+(const ZZn6&,const ZZn&);
    friend ZZn6 operator-(const ZZn6&,const ZZn6&);
    friend ZZn6 operator-(const ZZn6&,const ZZn&);
    friend ZZn6 operator-(const ZZn6&);

    friend ZZn6 operator*(const ZZn6&,const ZZn6&);
    friend ZZn6 operator*(const ZZn6&,const ZZn&);
    friend ZZn6 operator*(const ZZn&,const ZZn6&);

    friend ZZn6 operator*(int,const ZZn6&);
    friend ZZn6 operator*(const ZZn6&,int);

    friend ZZn6 operator/(const ZZn6&,const ZZn6&);
    friend ZZn6 operator/(const ZZn6&,const ZZn&);

    friend ZZn6 pow(const ZZn6&,const Big&);
    friend ZZn6 pow(const ZZn6&,const Big*);
    friend ZZn6 pow(const ZZn6*,const ZZn6&,const Big&);
    friend void precompute(const ZZn6&,ZZn6 *);
    friend ZZn6 conj(const ZZn6&);

    friend ZZn6 randn6(void);        // random ZZn6
    friend ZZn6 sqrt(const ZZn6&);   // square root - 0 if none exists

    friend BOOL operator==(const ZZn6& x,const ZZn6& y)
    {if (x.a[0]==y.a[0] && x.a[1]==y.a[1] && x.a[2]==y.a[2]
      && x.a[3]==y.a[3] && x.a[4]==y.a[4] && x.a[5]==y.a[5]) 
                                return TRUE; else return FALSE; }

    friend BOOL operator!=(const ZZn6& x,const ZZn6& y)
    {if (x.a[0]!=y.a[0] || x.a[1]!=y.a[1] || x.a[2]!=y.a[2]
      || x.a[3]!=y.a[3] || x.a[4]!=y.a[4] || x.a[5]!=y.a[5]) 
                                return TRUE; else return FALSE; }
#ifndef MR_NO_STANDARD_IO
    friend ostream& operator<<(ostream&,ZZn6&);
#endif
    ~ZZn6()  {}
};

#endif

