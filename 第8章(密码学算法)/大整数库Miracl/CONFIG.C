/*
 *   MIRACL utility program to automatically generate a mirdef.h file 
 *
 *   config.c
 *
 *   Compile WITHOUT any optimization
 *
 *   Run this with your computer/compiler configuration. It will
 *   attempt to create best-fit mirdef.h file for compiling the MIRACL 
 *   library. 
 *
 *   Generated are mirdef.tst - the suggested mirdef.h file, and 
 *   mirdef.lst which tells which modules should be included in the library 
 *   build.
 *
 *   Copyright (c) 1994-2002 Shamus Software Ltd.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int answer(void)
{
    char ch;
    scanf("%c",&ch);
    getchar();
    if (ch=='Y' || ch=='y') return 1;
    return 0;
}

int main()
{
    int chosen,utlen,dllen,flsh,stripped,standard,nofull,port,mant,r;
    int nbits,words,i,b,dlong,ibits,threaded,choice,selected,special;
    int chosen64,no64,step_size,double_type,rounding,lmant,dmant;
    int maxbase,end;
    char *ptr;
    char longlong[20],mfn[50];
    double s,magic;
    double x,y,eps;
    long double lx,ly,leps;
    FILE *fp,*fpl;
    printf("This program attempts to generate a mirdef.h file from your\n");
    printf("responses to some simple questions, and by some internal tests\n");
    printf("of its own.\n\n");
    printf("The most fundamental decision is that of the 'underlying type'\n");
    printf("that is the C data type to be used to store each digit of a\n");
    printf("big number. You input the number of bits in this type, and \n");
    printf("this program finds a suitable candidate (usually short, int or \n");
    printf("long). Typical values would be 16, 32 or perhaps 64. \n");
    printf("The bigger the better, but a good starting point would be to\n");
    printf("enter the native wordlength of your computer\n\n");
    printf("For your information:-\n");
    printf("The size of a short is %d bits\n",8*sizeof(short));
    printf("The size of an int  is %d bits\n",8*sizeof(int));
    printf("The size of a long  is %d bits\n",8*sizeof(long));

    eps=1.0;
    for (dmant=0;;dmant++)
    { /* IMPORTANT TO FOOL OPTIMIZER!!!!!! */
        x=1.0+eps;
        y=1.0;
        if (x==y) break;
        eps/=2.0;
    }

    printf("The size of a double is %d bits, its mantissa is %d bits\n",8*sizeof(double),dmant);

    leps=1.0; 
    for (lmant=0;;lmant++) 
    { /* IMPORTANT TO FOOL OPTIMIZER!!!!!! */ 
        lx=1.0+leps; 
        ly=1.0; 
        if (lx==ly) break; 
        leps/=2.0; 
    } 
    printf("The size of a long double is %d bits, its mantissa is %d bits\n",8*sizeof(long double),lmant);

    fp=fopen("mirdef.tst","w");
    fprintf(fp,"/*\n *   MIRACL compiler/hardware definitions - mirdef.h\n");
    fprintf(fp," *   Copyright (c) 1988-2002 Shamus Software Ltd.\n */\n\n");
    end=1;
    ptr=(char *)(&end);

    if ((int)(*ptr) == 1)
    {
         printf("\n    Little-endian processor detected\n\n");
         fprintf(fp,"#define MR_LITTLE_ENDIAN\n");
    }
    else 
    {
         printf("    Big-endian processor detected\n\n");
         fprintf(fp,"#define MR_BIG_ENDIAN\n");

    }
    printf("A double can be used as the underlying type. In rare circumstances\n");
    printf(" this may be optimal. Not recommended for beginners!\n\n");
    
    printf("Do you wish to use a double as the underlying type? (Y/N)?");
    double_type=answer();
    nofull=0;
    chosen=0;
    chosen64=0;
    no64=0;
    if (double_type)
    {
      fprintf(fp,"#define MIRACL %d\n",8*sizeof(double));
      fprintf(fp,"#define mr_utype double\n");
      fprintf(fp,"#define mr_dltype long double\n");
      fprintf(fp,"#define MR_NOFULLWIDTH\n");
      fprintf(fp,"#define MR_FP\n");
      nofull=1;
    }
    else
    {
      printf("\nNow enter number of bits in underlying type= ");
      scanf("%d",&utlen);
      getchar();
      printf("\n");
      if (utlen!=8 && utlen!=16 && utlen!=32 && utlen!=64) 
      {
        printf("Non-standard system - this program cannot help!\n");
        fclose(fp);
        exit(0);
      }

      fprintf(fp,"#define MIRACL %d\n",utlen);

      if (utlen==8)
      {
        printf("    underlying type is a char\n");
        fprintf(fp,"#define mr_utype char\n");
        chosen=1;
      }
      if (!chosen && utlen==8*sizeof(short))
      {
        printf("    underlying type is a short\n");
        fprintf(fp,"#define mr_utype short\n");
        chosen=1;
      }
      if (!chosen && utlen==8*sizeof(int))
      {
        printf("    underlying type is an int\n");
        fprintf(fp,"#define mr_utype int\n");
        chosen=1;
      }  
      if (!chosen && utlen==8*sizeof(long))
      {
        printf("    underlying type is a long\n");
        fprintf(fp,"#define mr_utype long\n");
        chosen=1;
      }
      dlong=0;
      if (!chosen && utlen==16*sizeof(long))
      {
        printf("\nDoes compiler support a %d bit unsigned type? (Y/N)?", 
              utlen);
        dlong=answer();
        if (dlong)
        {
            printf("Is it called long long? (Y/N)?");
            if (!answer())
            {
                printf("Enter its name : ");
                scanf("%s",longlong);
                getchar();
            }
            else strcpy(longlong,"long long");
            printf("    underlying type is a %s\n",longlong);
            fprintf(fp,"#define mr_utype %s\n",longlong);
            chosen=1;
            if (64==utlen && !chosen64)
            { 
                printf("    64 bit unsigned type is an unsigned %s\n",longlong);
                fprintf(fp,"#define mr_unsign64 unsigned %s\n",longlong);
                chosen64=1;
            }
        }
        else
        {
            if (utlen==64) no64=1; /* there is no 64-bit type */
        }
      }

      if (!chosen)
      {
        printf("No suitable underlying type could be found\n");
        fclose(fp);
        exit(0);
      }
    }
/* Now try to find a 32-bit unsigned type */

    fprintf(fp,"#define MR_IBITS %d\n",8*sizeof(int));
    fprintf(fp,"#define MR_LBITS %d\n",8*sizeof(long)); 

    chosen=0;
    if (32==8*sizeof(unsigned short))
    {
        printf("    32 bit unsigned type is a unsigned short\n");
        fprintf(fp,"#define mr_unsign32 unsigned short\n");
        chosen=1;
    }
    if (!chosen && 32==8*sizeof(unsigned int))
    {
        printf("    32 bit unsigned type is an unsigned int\n");
        fprintf(fp,"#define mr_unsign32 unsigned int\n");
        chosen=1;
    }
    if (!chosen && 32==8*sizeof(long))
    {
        printf("    32 bit unsigned type is an unsigned long\n");
        fprintf(fp,"#define mr_unsign32 unsigned long\n");
        chosen=1;
    }

    if (!chosen)
    {
        printf("No suitable 32 bit unsigned type could be found\n");
        fclose(fp);
        exit(0);
    }

    fpl=fopen("miracl.lst","w");

    fprintf(fpl,"mrcore.c\n");
    fprintf(fpl,"mrarth0.c\n");
    fprintf(fpl,"mrarth1.c\n");
    fprintf(fpl,"mrarth2.c\n");
    fprintf(fpl,"mralloc.c\n");
    fprintf(fpl,"mrsmall.c\n");
    fprintf(fpl,"mrio1.c\n");
    fprintf(fpl,"mrio2.c\n");
    fprintf(fpl,"mrgcd.c\n");
    fprintf(fpl,"mrjack.c\n");
    fprintf(fpl,"mrxgcd.c\n");
    fprintf(fpl,"mrarth3.c\n");
    fprintf(fpl,"mrrand.c\n");
    fprintf(fpl,"mrprime.c\n");
    fprintf(fpl,"mrcrt.c\n");
    fprintf(fpl,"mrscrt.c\n");
    fprintf(fpl,"mrmonty.c\n");
    fprintf(fpl,"mrpower.c\n");
    fprintf(fpl,"mrcurve.c\n");
    fprintf(fpl,"mrfast.c\n");
    fprintf(fpl,"mrlucas.c\n");
    fprintf(fpl,"mrshs.c\n");
    fprintf(fpl,"mrshs256.c\n");
    fprintf(fpl,"mraes.c\n");
    fprintf(fpl,"mrstrong.c\n");
    fprintf(fpl,"mrbrick.c\n");
    fprintf(fpl,"mrebrick.c\n");

/* are some of the built-in types 64-bit? */

    if (64==8*sizeof(int))
    {
        printf("    64 bit unsigned type is an unsigned int\n");
        fprintf(fp,"#define mr_unsign64 unsigned int\n");
        chosen64=1;
    }
    if (!chosen64 && 64==8*sizeof(long))
    {
        printf("    64 bit unsigned type is an unsigned long\n");
        fprintf(fp,"#define mr_unsign64 unsigned long\n");
        chosen64=1;
    }

/* Now try to find a double length type */
    dlong=0;

    if (!double_type)
    {
      dllen=2*utlen;
      chosen=0;
      if (!chosen && dllen==8*sizeof(short))
      {
        printf("    double length type is a short\n");
        fprintf(fp,"#define mr_dltype short\n");
        chosen=1;
      }
      if (!chosen && dllen==8*sizeof(int))
      {
        printf("    double length type is an int\n");
        fprintf(fp,"#define mr_dltype int\n");
        chosen=1;
      }
      if (!chosen && dllen==8*sizeof(long))
      {
        printf("    double length type is a long\n");
        fprintf(fp,"#define mr_dltype long\n");
        chosen=1;
      }
      if (!chosen && dllen==16*sizeof(long))
      {
        printf("\nDoes compiler support a %d bit integer type? (Y/N)?", 
              dllen);
        dlong=answer();
       
        if (dlong)
        {
            printf("Is it called long long? (Y/N)?");
            if (!answer())
            {
                printf("Enter its name : ");
                scanf("%s",longlong);
                getchar();
            }
            else strcpy(longlong,"long long");

            printf("    double length type is a %s\n",longlong);
            fprintf(fp,"#define mr_dltype %s\n",longlong);
            chosen=1;
            if (64==dllen && !chosen64)
            { 
                printf("    64 bit unsigned type is an unsigned %s\n",longlong);
                fprintf(fp,"#define mr_unsign64 unsigned %s\n",longlong);
                chosen64=1;
            }
        }
        else
        {
            if (dllen==64) no64=1; /* there is no 64-bit type */
        }
      }
    }
    else dlong=1;

    if (!no64 && !chosen64)
    { /* Still no 64-bit type, but not ruled out either */
        printf("\nDoes compiler support a 64-bit integer type? (Y/N)?");
        dlong=answer();
       
        if (dlong)
        {
            printf("Is it called long long? (Y/N)?");
            if (!answer())
            {
                printf("Enter its name : ");
                scanf("%s",longlong);
                getchar();
            }
            else strcpy(longlong,"long long");

            printf("    64-bit type is a %s\n",longlong);
            fprintf(fp,"#define mr_unsign64 unsigned %s\n",longlong);
            chosen64=1;
        }
    }

    if (chosen64) fprintf(fpl,"mrshs512.c\n");

    port=0;
    if (chosen)
    {
        printf("\nDo you want a C-only version of MIRACL (Y/N)?");
        port=answer();
        if (port) fprintf(fp,"#define MR_NOASM\n");
    }
    rounding=0;
    if (double_type)
    {
#ifdef __TURBOC__
        rounding=1;
#endif        
#ifdef _MSC_VER
        rounding=1;
#endif        
#ifdef __GNUC__
        rounding=1;
#endif        
        if (!rounding)
        {
          printf("It will help if rounding control can be exercised on doubles\n");
          printf("Can you implement this in mrarth1.c??  (Y/N)?");
          if (answer()) rounding=1;
        }
        if (rounding) 
        {
            fprintf(fp,"#define MR_FP_ROUNDING\n");
            magic=1.0;
            for (i=0;i<lmant-2;i++) magic*=2.0;
            magic+=2*magic;
            fprintf(fp,"#define MR_MAGIC %lf\n",magic);
        } 
    }

    printf("\nDo you want support for flash arithmetic? (Y/N)?");
    flsh=answer();
    if (flsh)
    { /* calculate size of mantissa in bits */
        eps=1.0;
        for (mant=0;;mant++)
        { /* IMPORTANT TO FOOL OPTIMIZER!!!!!! */
            x=1.0+eps;
            y=1.0;
            if (x==y) break;
            eps/=2.0;
        }
        mant--;
        fprintf(fp,"#define MR_FLASH %d\n",mant);  

        fprintf(fpl,"mrflash.c\n");
        fprintf(fpl,"mrfrnd.c\n");
        fprintf(fpl,"mrdouble.c\n");
        fprintf(fpl,"mrround.c\n");
        fprintf(fpl,"mrbuild.c\n");
        fprintf(fpl,"mrflsh1.c\n");
        fprintf(fpl,"mrpi.c\n");
        fprintf(fpl,"mrflsh2.c\n");
        fprintf(fpl,"mrflsh3.c\n");
        fprintf(fpl,"mrflsh4.c\n");

    }

    printf("Do you want stripped-down version (smaller - no error messages) (Y/N)?");
    stripped=answer();
    if (stripped) fprintf(fp,"#define MR_STRIPPED_DOWN\n");

    printf("Do you want multi-threaded version of MIRACL\n");
    printf("Not recommended for program development - read the manual (Y/N)?");
    threaded=answer();

    if (threaded)
    {
     printf("Do you want generic portable threading support (No C++) (Y/N)?");
     choice=answer();
     if (choice) fprintf(fp,"#define MR_GENERIC_MT\n");
     if (!choice) 
     {
         printf("Do you want multi-threaded support for MS Windows (Y/N)?");
         choice=answer();
         if (choice) fprintf(fp,"#define MR_WINDOWS_MT\n");
     }
     if (!choice)
     {
         printf("Do you want multi-threaded support for Unix (Y/N)?");
         choice=answer();
         if (choice) fprintf(fp,"#define MR_UNIX_MT\n");
 
     }
    }
    printf("Does your development environment support standard screen/keyboard I/O?\n");
    printf("(It doesn't for example in MS Windows, and embedded applications)\n");
    printf("If in doubt, answer Yes (Y/N)?");
    standard=answer();
    if (!standard) fprintf(fp,"#define MR_NO_STANDARD_IO\n");

    printf("Does your development environment support standard file I/O?\n");
    printf("(It doesn't for example in an embedded application)\n");
    printf("If in doubt, answer Yes (Y/N)?");
    standard=answer();
    if (!standard) fprintf(fp,"#define MR_NO_FILE_IO\n");

    if (!chosen && !nofull)
    {
        printf("\n\nDo you for some reason NOT want to use a full-width number base?\n");
        printf("\nYou may not if your processor instruction set does not support\n");
        printf("%d-bit UNSIGNED multiply and divide instructions.\n",utlen);
        printf("If NOT then a full-width number base will be difficult and \n");
        printf("slow to implement, which is a pity, because its normally faster\n"); 
        printf("If for some other reason you don't want to use a full-width\n");
        printf("number base, (no muldvd()/muldvd2()/muldvm() available?), answer Yes\n");
        printf("\nAnswer (Y/N)?");
        nofull=answer();
        if (nofull) 
        {
            printf("\nRemember to use mirsys(...,MAXBASE), or somesuch, in your programs\n");
            printf("as mirsys(...,0); will generate an 'Illegal Number base' error\n");
            fprintf(fp,"#define MR_NOFULLWIDTH\n");
        }
    }
    selected=special=0;
    if (!nofull)
    {
     fprintf(fpl,"mrecgf2m.c\n");

     fprintf(fp,"#define MAXBASE ((mr_small)1<<(MIRACL-1))\n");
     printf("\nDo you wish to use the Karatsuba/Comba/Montgomery method\n");
     printf("for modular arithmetic - as used by exponentiation\n");
     printf("cryptosystems like RSA.\n"); 
     if (port)
     {
      printf("This method may be faster than the standard method when\n");
      printf("using larger moduli, or if your processor has no \n");
      printf("unsigned integer multiply/divide instruction in its\n");
      printf("instruction set. This is true of some popular RISC computers\n");
     }
     else
     {
      printf("This method is probably fastest om most processors which\n");
      printf("which support unsigned mul and a carry flag\n");
      printf("NOTE: your compiler must support in-line assembly,\n");
      printf("and you must be able to supply a suitable .mcs file\n");
      printf("like, for example, ms86.mcs for pentium processors\n");
     }
     printf("\nAnswer (Y/N)?");
     r=answer();
     if (r)
     {
      printf("\nThis method can only be used with moduli whose length in\n");
      printf("bits can be represented as %d*(step size)*2^n, for any value\n",utlen);
      printf("of n. For example if you input a step size of 8, then \n");
      printf("moduli of 256, 512, 1024 bits etc will use this fast code\n");
      if (port)
       printf("In this case case a step size of 4 is probably optimal\n");
      else
      {
       printf("The best step size can be determined by experiment, but\n");
       printf("larger step sizes generate more code. For the Pentium 8 is \n"); 
       printf("optimal. For the Pentium Pro/Pentium II 16 is optimal.\n"); 
       printf("If in doubt, set to 8\n"); 
      }
      step_size=0;
      while (step_size<2 || step_size>16)
      {
          printf("Enter step size = ");
          scanf("%d",&step_size);
          getchar();
      }

      printf("\nTo create the file MRKCM.C you must next execute\n");
      if (port) printf("MEX %d C MRKCM\n",step_size);
      else
      {
       printf("MEX %d <file> MRKCM\n",step_size);
       printf("where <file> is the name of the macro .mcs file (e.g. ms86)\n");
      }

      printf("\nSpecial routines for modular multiplication will now be\n");
      printf("automatically be invoked when, for example, powmod() is called\n");
      printf("\nRemember to use a full-width base in your programs\n");
      printf("by calling mirsys(..,0) or mirsys(..,256) at the start of the program\n");
      
      fprintf(fp,"#define MR_KCM %d\n",step_size);          
      fprintf(fpl,"mrkcm.c\n");
      selected=1;
     }
     else
     {
      printf("\nDo you want to create a Comba fixed size modular\n");
      printf("multiplier, for faster modular multiplication with\n");
      printf("smaller moduli. Can generates a *lot* of code \n");
      printf("Useful particularly for Elliptic Curve cryptosystems.\n");
      printf("\nAnswer (Y/N)?");  
      r=answer();
      if (r)
      {
       step_size=0;
       while (step_size<2 || step_size>32)
       {
          printf("Enter modulus size in bits = ");
          scanf("%d",&nbits);
          getchar();
          step_size=nbits/utlen;
          if ((nbits%utlen)!=0) step_size++;
       }
       printf("Are you willing to implement a \"special\" fast method\n");
       printf("for modular reduction, for a particular modulus\n");
       printf("See mrcomba.tpl. If in any doubt answer No (Y/N)?");
       r=answer();
       if (r) 
       {
         special=1;
         printf("\nYou must first edit the routine comba_redc() in MRCOMBA.TPL");
       }
       printf("\nTo create the file MRCOMBA.C you must next execute\n");
       if (port) printf("MEX %d C MRCOMBA\n",step_size);
       else
       {
        printf("MEX %d <file> MRCOMBA\n",step_size);
        printf("where <file> is the name of the macro .mcs file (e.g. ms86)\n");
       }

       fprintf(fp,"#define MR_COMBA %d\n",step_size);
       if (special) fprintf(fp,"#define MR_SPECIAL\n");
       fprintf(fpl,"mrcomba.c\n");

       printf("\nSpecial routines for modular multiplication will now \n");
       printf("automatically be invoked when, for example, powmod() is called\n");
       printf("\nRemember to use a full-width base in your programs\n");
       printf("by calling mirsys(..,0) or mirsys(,..,256) at the start of the program\n");
       selected=1;
      }

     }
    }
    else
    {
     if (!double_type) fprintf(fp,"#define MAXBASE ((mr_small)1<<(MIRACL-2))\n");
    }

    if (double_type)
    {
     maxbase=0;
#ifdef __TURBOC__ 
     if (!port && !selected)
     {
      printf("\nDoes your computer have a Pentium processor\n");
      printf("and do you wish to exploit its built-in FP coprocessor\n");
      printf("NOTE: this may not be optimal for Pentium Pro or Pentium II\n");
      printf("Supported only for 80x86 processors, and Borland C Compilers\n");
      printf("This is a little experimental - so use with care\n");
      printf("Answer (Y/N)?");
      r=answer();
      if (r) 
      {
          printf("Enter (maximum) modulus size in bits = ");
          scanf("%d",&nbits);
          getchar();
          b=31;
          do {
                  b--;
                  r=64-b-b;
                  s=1.0;
                  for (i=0;i<r;i++) s*=2.0;
                  s*=b;
          } while (s<=2*nbits);
          s=1; for (i=0;i<b;i++) s*=2;
          printf("\nDo you wish to generate variable length looping code, or\n");
          printf("fixed length unrolled code? The former can be used with any\n");
          printf("modulus less than the maximum size specified above. The latter will\n");
          printf("only work with a fixed modulus of that size, but is usually a bit\n");
          printf("faster, although it can generate a *lot* of code for larger moduli.\n"); 

          printf("\nAnswer Yes for looping code(Y/N)?");
          r=answer();
          if (r)
          {
           fprintf(fp,"#define MR_PENTIUM -%d\n",nbits/b+1);

           fprintf(fpl,"mr87v.c\n");

           printf("Make sure to compile and link into your program the module MR87V.C\n");
           }
          else
          {
           fprintf(fp,"#define MR_PENTIUM %d\n",nbits/b+1);
           fprintf(fpl,"mr87f.c\n");
           printf("Make sure to compile and link into your program the module MR87F.C\n");
 
          }
          printf("\nSpecial fast routines for modular multiplication will now be\n");
          printf("automatically be invoked when, for example, powmod() is called\n");
          printf("\nIt is *vital* to use the appropriate number base, so\n");
          printf("you *must* now call mirsys(...,MAXBASE) at the start of your program\n");
          fprintf(fp,"#define MAXBASE %lf\n",s);
          maxbase=1;
      }
     }
#endif
     if (!maxbase)
     {
         s=1.0; 
         for (i=0;i<dmant-1;i++) /* extra bit "spare" so that 2 can be added */
         {
             if (i+i+1>=lmant) break;
             s*=2.0;
         }  
         fprintf(fp,"#define MAXBASE %lf\n",s);
     }
    }
    if (!port)
    {
        if (!dlong) printf("\nYou must now provide an assembly language file mrmuldv.c,\n");
        else printf("\nYou must now provide an assembly or C file mrmuldv.c,\n");
        if (!nofull)
        printf("containing implementations of muldiv(), muldvd(), muldvd2() and muldvm()\n");
        else
        {
            printf("containing an implementation of muldiv()\n");
            if (rounding) printf("..and imuldiv()\n");
        }
        if (!dlong)
            printf("Check mrmuldv.any - an assembly language version may be\n");
        else
            printf("Check mrmuldv.any - a C or assembly language version is\n");
        printf("there already\n");
      
        fprintf(fpl,"mrmuldv.c\n");

    }
    printf("\nA file mirdef.tst has been generated. If you are happy with it,\n");
    printf("rename it to mirdef.h and use for compiling the MIRACL library.\n");

    printf("A file miracl.lst has been generated that includes all the \n");
    printf("files to be included in this build of the MIRACL library.\n");


    fprintf(fpl,"\nCompile the above with -O2 optimization\n");
    if (threaded) 
      fprintf(fpl,"Also use appropriate flag for multi-threaded compilation\n");

    if (!port)
    {
      fprintf(fpl,"Note that mrmuldv.c file may be pure assembly, so may \n");
      fprintf(fpl,"be renamed to mrmuldv.asm or mrmuldv.s, and assembled \n");
      fprintf(fpl,"rather than compiled\n");
    }
    fclose(fp);
    fclose(fpl);

    return 0;
}

