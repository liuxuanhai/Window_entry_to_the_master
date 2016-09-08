/*
 *   MIRACL Comba's method for ultimate speed modular multiplication
 *   mrcomba.tpl 
 *
 *   See "Exponentiation Cryptosystems on the IBM PC", IBM Systems
 *   Journal Vol. 29 No. 4 1990. Comba's method has been extended to 
 *   implement Montgomery reduction. 
 *
 *   Here the inner loops of the basic multiplication, squaring and 
 *   Montgomery's redc() functions are completely unravelled, and 
 *   reorganised for maximum possible speed. 
 *
 *   This approach is recommended for maximum speed where parameters
 *   are fixed and compute resources are constrained. The processor must 
 *   support an unsigned multiply instruction, and should have a carry flag.
 *
 *   This file is a template. To fill in the gaps and create mrcomba.c, 
 *   you must run the mex.c program to insert the C or assembly language 
 *   macros from the appropriate .mcs file. For use with C MR_NOASM must
 *   be defined in mirdef.h
 *
 *   This method would appear to be particularly useful for implementing 
 *   fast Elliptic Curve Cryptosystems over GF(p) and fast 1024-bit RSA
 *   decryption.
 *
 *   The #define MR_COMBA in mirdef.h determines the FIXED size of 
 *   modulus to be used. This *must* be determined at compile time. 
 *
 *   Note that this module can generate a *lot* of code for large values 
 *   of MR_COMBA. This should have a maximum value of 8-16. Any larger 
 *   that and you should define MR_KCM instead - see mrkcm.tpl
 *
 *   Note that on some processors it is *VITAL* that arrays be aligned on 
 *   4-byte boundaries
 *
 *   Copyright (c) 1988-2001 Shamus Software Ltd.
 */

#include <stdio.h>     
#include <miracl.h>    

#ifdef MR_COMBA
#if INLINE_ASM == 1    
#define N 2
#define POINTER WORD PTR  
#define PBP bp   
#define PBX bx   
#define PSI si   
#define PDI di   
#define DSI si   
#define DDI di   
#define DBP bp   
#define DAX ax   
#define DCX cx   
#define DDX dx   
#endif   
 
#if INLINE_ASM == 2    
#define N 4
#define POINTER DWORD PTR   
#define PBP bp   
#define PBX bx   
#define PSI si   
#define PDI di   
#define DSI esi  
#define DDI edi  
#define DBP ebp  
#define DAX eax  
#define DCX ecx  
#define DDX edx  
#endif           
  
#if INLINE_ASM == 3    
#define N 4
#define POINTER DWORD PTR   
#define PBP ebp   
#define PBX ebx   
#define PSI esi   
#define PDI edi   
#define DSI esi  
#define DDI edi  
#define DBP ebp  
#define DAX eax  
#define DCX ecx  
#define DDX edx  
#endif           
  
/* NOTE! z must be distinct from x and y */

void comba_mult(_MIPD_ big x,big y,big z) 
{ /* comba multiplier */
    int i;
    mr_small *a,*b,*c;
    big w;
#ifdef MR_NOASM 
    register mr_small extra,s0,s1;
    mr_large pp1,pp2,sum;
#endif
#ifndef MR_GENERIC_MT
    miracl *mr_mip=get_mip();
#endif
    w=mr_mip->w0; 
    for (i=2*MR_COMBA;i<(int)(w->len&MR_OBITS);i++) w->w[i]=0;
  /*  zero(w); */
    w->len=2*MR_COMBA;
    a=x->w; b=y->w; c=w->w;
/*** MULTIPLY ***/      /* multiply a by b, result in c */

    if (w!=z) copy (w,z); 
}   
 
/* NOTE! z and x must be distinct */

void comba_square(_MIPD_ big x,big z)  
{ /* super comba squarer */
    int i;
    mr_small *a,*c;
    big w;
#ifdef MR_NOASM
    register mr_small extra,s0,s1;
    mr_large pp1,pp2,sum;
#endif
#ifndef MR_GENERIC_MT
    miracl *mr_mip=get_mip();
#endif
    w=mr_mip->w0;       
    for (i=2*MR_COMBA;i<(int)(w->len&MR_OBITS);i++) w->w[i]=0;  
  /*  zero(w); */
    w->len=2*MR_COMBA;
    a=x->w; c=w->w;
/*** SQUARE ***/    /* squares a, result in b */

    if (w!=z) copy (w,z);      
}                        
                         
/* NOTE! t and z must be distinct! */

void comba_redc(_MIPD_ big t,big z)     
{  /* super comba Montgomery redc() function */                      
    mr_small ndash,carry;
#ifdef MR_NOASM
    register mr_small sp,extra,s0,s1;
    mr_large pp1,pp2,sum,u;
#endif
    unsigned int i;
    big w,modulus;
    mr_small *a,*b;
#ifndef MR_GENERIC_MT
    miracl *mr_mip=get_mip();
#endif
#ifdef MR_SPECIAL

/* !!! Implement here a "special" fast method for modular reduction,
   for a particular modulus. Implemented here for 2^192-2^64-1       
   and 2^224-2^96+1 on a 32 bit processor.
   See for example "Software Implementation of the NIST Elliptic
   Curves Over Prime Fields", Brown et al., Report #36, 2000 available
   from www.cacr.math.uwaterloo.ca 

   The generated code can be manually optimised further.....
*/
    int overshoot;
    mr_small k[MR_COMBA];
    mr_small *c;
    modulus=mr_mip->modulus;     
    for (i=MR_COMBA;i<(int)(z->len&MR_OBITS);i++) z->w[i]=0;
 /*      zero(z);   */
    z->len=MR_COMBA;

#if MR_COMBA == 6

/* Special Code for 2^192-2^64-1 - assuming 32-bit processor */

    a=t->w; b=k; c=z->w;
    k[0]=k[2]=a[6]; k[1]=k[3]=a[7]; k[4]=k[5]=0; 
    
/*** ADDITION ***/
    overshoot=carry;  
    a=c;  c=t->w;
    k[0]=k[1]=0; k[2]=k[4]=c[8]; k[3]=k[5]=c[9];

/*** INCREMENT ***/
    overshoot+=carry;
    k[0]=k[2]=k[4]=c[10]; k[1]=k[3]=k[5]=c[11];
                       
/*** INCREMENT ***/
    overshoot+=carry;
    b=modulus->w;
    while(overshoot>0)
    {
/*** DECREMENT ***/
        overshoot-=carry;
    }
    if (z->w[MR_COMBA-1]>=modulus->w[MR_COMBA-1])
    {
        if (compare(z,modulus)>=0)
        {
/*** DECREMENT ***/
        }
    }
    if (z->w[MR_COMBA-1]==0) mr_lzero(z);

#endif

#if MR_COMBA == 7
/* Special Code for 2^224-2^96+1 - assuming 32-bit processor */

    a=t->w; b=k; c=z->w;
    k[0]=k[1]=k[2]=0; k[3]=a[7]; k[4]=a[8]; k[5]=a[9]; k[6]=a[10];

/*** ADDITION ***/
    overshoot=carry;
    a=c; c=t->w;
    k[0]=k[1]=k[2]=k[6]=0; k[3]=c[11]; k[4]=c[12]; k[5]=c[13];

/*** INCREMENT ***/
    overshoot+=carry;
    k[0]=c[7]; k[1]=c[8]; k[2]=c[9]; k[3]=c[10]; k[4]=c[11]; k[5]=c[12]; k[6]=c[13];
    
/*** DECREMENT ***/
    overshoot-=carry;
    k[0]=c[11]; k[1]=c[12]; k[2]=c[13]; k[3]=k[4]=k[5]=k[6]=0;

/*** DECREMENT ***/
    overshoot-=carry;
    b=modulus->w;
    while (overshoot>0)
    {
/*** DECREMENT ***/
        overshoot-=carry;
    }
    while (overshoot<0)
    {
/*** INCREMENT ***/
        overshoot+=carry;
    }
    if (z->w[MR_COMBA-1]>=modulus->w[MR_COMBA-1])
    {
        if (compare(z,modulus)>=0)
        {
/*** DECREMENT ***/
        }
    }
    if (z->w[MR_COMBA-1]==0) mr_lzero(z);

#endif

#if MR_COMBA == 17

/* Special Code for 2^521-1 - assuming 32-bit processor */

/* split t into 521-bit halves, low half in a, high half in b */

    a=t->w; b=k; c=z->w;

    for (i=0;i<=16;i++)
        b[i]=(a[i+16]>>9)|(a[i+17]<<23);

    b[16]|=(-(a[16]>>9)<<9); /* clever stuff! Set top part of b[16] to minus  *
                              * top part of a[16]. When added they cancel out */

/*** ADDITION ***/
                             /* ignore carry=1 */
    a=z->w;                   
    b=modulus->w;

    if (z->w[MR_COMBA-1]>=modulus->w[MR_COMBA-1])
    {
        if (compare(z,modulus)>=0)
        {
/*** DECREMENT ***/
        }
    }
    if (z->w[MR_COMBA-1]==0) mr_lzero(z);
#endif

#else
    modulus=mr_mip->modulus;  
    ndash=mr_mip->ndash;
    w=mr_mip->w0;
    if (t!=w) copy(t,w);       
    w->len=2*MR_COMBA+1;
    a=w->w; b=modulus->w;

/*** REDC ***/      /* reduces a mod b */
    
    for (i=MR_COMBA+1;i<(int)(z->len&MR_OBITS);i++) z->w[i]=0;
 /*   zero(z); */
    z->len=MR_COMBA+1;
    for (i=0;i<=MR_COMBA;i++) z->w[i]=w->w[i+MR_COMBA];
    mr_lzero(z);
    if (compare(z,modulus)>=0) 
    {
        a=z->w; b=modulus->w;
/*** DECREMENT ***/    
        z->w[MR_COMBA]=0;  z->len=MR_COMBA;
        mr_lzero(z);
    }
#endif
} 

void comba_add(_MIPD_ big x,big y,big w)
{ /* fast modular addition */
    int i;
    big modulus;
    mr_small *a,*b,*c;
    mr_small carry;  
#ifdef MR_NOASM
    mr_large u;
#endif
#ifndef MR_GENERIC_MT
    miracl *mr_mip=get_mip();
#endif
    modulus=mr_mip->modulus;
    if (w!=x && w!=y) 
    {
        for (i=MR_COMBA;i<(w->len&MR_OBITS);i++) w->w[i]=0;
        /* zero(w); */
    }
    
    a=x->w; b=y->w; c=w->w;
/*** ADDITION ***/        /* add a and b, result in c */
    w->len=MR_COMBA;

    if (!carry && w->w[MR_COMBA-1]<modulus->w[MR_COMBA-1])
    {
         
        if (w->w[MR_COMBA-1]==0) mr_lzero(w);   
        return;
    }
    if (carry || compare(w,modulus)>=0)
    {

        a=w->w; b=modulus->w;
/*** DECREMENT ***/        /* decrement b from a */
    
    }
    if (w->w[MR_COMBA-1]==0) mr_lzero(w);   
 
}

void comba_sub(_MIPD_ big x,big y,big w)
{ /* fast modular subtraction */
    int i;
    big modulus;
    mr_small *a,*b,*c;
    mr_small carry;  
#ifdef MR_NOASM
    mr_large u;
#endif

#ifndef MR_GENERIC_MT
    miracl *mr_mip=get_mip();
#endif
    modulus=mr_mip->modulus;
    if (x!=w && y!=w) 
    {
        for (i=MR_COMBA;i<(w->len&MR_OBITS);i++) w->w[i]=0;   
        /* zero(w); */
    }

    a=x->w; b=y->w; c=w->w;
/*** SUBTRACTION ***/

    if (carry)
    {
        a=w->w; b=modulus->w; 
/*** INCREMENT ***/        /* add a and b, result in c */
    
    }
    w->len=MR_COMBA;
    if (w->w[MR_COMBA-1]==0) mr_lzero(w); 
}

#endif


