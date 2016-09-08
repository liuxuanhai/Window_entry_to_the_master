/*
 *    MIRACL  C++  functions monty.cpp
 *
 *    AUTHOR  :    M. Scott
 *             
 *    PURPOSE :    Implementation of class ZZn functions using Montgomery
 *                 representation
 *    NOTE    :    Must be used in conjunction with big.h and big.cpp
 *
 *    Copyright (c) 1988-1997 Shamus Software Ltd.
 */

#include <monty.h>

BOOL ZZn::iszero() const
{return fn.iszero(); }

ZZn operator-(const ZZn& b)
{ZZn x=b; nres_negate(x.fn,x.fn); return x;}

ZZn operator+(const ZZn& b,int i)
{ZZn abi=b; nres_modadd(abi.fn,nres((Big)i),abi.fn); return abi;}
ZZn operator+(int i, const ZZn& b)
{ZZn aib=b; nres_modadd(aib.fn,nres((Big)i),aib.fn); return aib;}
ZZn operator+(const ZZn& b1, const ZZn& b2)
{ZZn abb=b1; nres_modadd(abb.fn,b2.fn,abb.fn); return abb;}

ZZn operator-(const ZZn& b, int i)
{ZZn mbi=b; nres_modsub(mbi.fn,nres((Big)i),mbi.fn); return mbi;}
ZZn operator-(int i, const ZZn& b)
{ZZn mib=b; nres_modsub(mib.fn,nres((Big)i),mib.fn); return mib;}
ZZn operator-(const ZZn& b1, const ZZn& b2)
{ZZn mbb=b1;nres_modsub(mbb.fn,b2.fn,mbb.fn);  return mbb;}

ZZn operator*(const ZZn& b,int i)
{ZZn xbb=b; nres_premult(xbb.fn,i,xbb.fn); return xbb;}
ZZn operator*(int i, const ZZn& b)
{ZZn xbb=b; nres_premult(xbb.fn,i,xbb.fn); return xbb;}
ZZn operator*(const ZZn& b1, const ZZn& b2)
{ZZn xbb=b1; nres_modmult(xbb.fn,b2.fn,xbb.fn); return xbb;}


ZZn operator/(const ZZn& b1, int i)
{ZZn z=b1; nres_moddiv(z.fn,nres((Big)i),z.fn); return z;}
ZZn operator/(int i, const ZZn& b2)
{ZZn z=nres((Big)i); nres_moddiv(z.fn,b2.fn,z.fn); return z;}
ZZn operator/(const ZZn& b1, const ZZn& b2)
{ZZn z=b1; nres_moddiv(z.fn,b2.fn,z.fn); return z;}

ZZn pow( const ZZn& b1, const Big& b2)
{ZZn z;z.fn=nres_pow(b1.fn,b2);return z;}

ZZn pow( const ZZn& b,int i)
{ZZn z; z.fn=nres_pow(b.fn,(Big)i); return z;}

ZZn pow( const ZZn& b1, const Big& b2, const ZZn& b3,const Big& b4)
{ZZn z;z.fn=nres_pow2(b1.fn,b2,b3.fn,b4); return z;}

ZZn randn(void)
{ZZn z; Big p=get_mip()->modulus; z=rand(p); return z;}

BOOL qr(ZZn& x)
{ Big p=get_mip()->modulus; if (jacobi(Big(x),p)==1) return TRUE; return FALSE; }

BOOL qnr(ZZn& x)
{ Big p=get_mip()->modulus; if (jacobi(Big(x),p)==-1) return TRUE; return FALSE;}

ZZn getA(void)
{
 ZZn w; 
 if (get_mip()->Asize<MR_TOOBIG) w=get_mip()->Asize;
 else w=get_mip()->A;
 return w;
}

ZZn getB(void)
{ 
 ZZn w; 
 if (get_mip()->Bsize<MR_TOOBIG) w=get_mip()->Bsize;
 else w=get_mip()->B;
 return w;
}

ZZn pow(int n,ZZn *x,Big *y)
{
   ZZn z;
   int i;
   Big *a=new Big[n];
   for (i=0;i<n;i++) 
      a[i]=x[i].fn;
   z.fn=nres_pown(n,a,y);

   delete [] a;
   return z;
}

ZZn luc( const ZZn& b1, const Big& b2, ZZn *b3)
{ZZn z; if (b3!=NULL) z.fn=nres_luc(b1.fn,b2,&(b3->fn)); 
        else          z.fn=nres_luc(b1.fn,b2); 
 return z;}


ZZn sqrt(const ZZn& b)
{ZZn z; z.fn=nres_sqrt(b.fn); return z;}


