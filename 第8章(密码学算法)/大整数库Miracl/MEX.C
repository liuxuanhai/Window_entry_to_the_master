/* mex.c
 *
 * Updated to allow emission of scheduled code. 
 *
 * Macro EXpansion program.
 * Expands Macros from a .mcs file into a .tpl file to create a .c file
 *
 *  Copyright (c) 2002 Shamus Software Ltd.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int BOOL;
#define FALSE 0
#define TRUE 1

/* Define Algorithms */

#define MULTIPLY 0
#define MULTUP 1
#define SQUARE 2
#define REDC 3
#define ADDITION 4
#define INCREMENT 5
#define SUBTRACTION 6
#define DECREMENT 7
#define SUMMATION 8
#define INCREMENTATION 9
#define DECREMENTATION 10

/* Define Macros */

#define MUL_START       0
#define STEP            1
#define STEP1M          2
#define STEP1A          3
#define STEP2M          4
#define STEP2A          5
#define MFIN            6
#define MUL_END         7
#define LAST            8
#define SQR_START       9
#define DSTEP           10
#define DSTEP1M         11
#define DSTEP1A         12
#define DSTEP2M         13
#define DSTEP2A         14
#define SELF            15
#define SFIN            16
#define SQR_END         17
#define REDC_START      18
#define RFINU           19
#define RFIND           20
#define REDC_END        21
#define ADD_START       22
#define ADD             23
#define ADD_END         24
#define SUB_START       25
#define SUB             26
#define SUB_END         27
#define INC_START       28
#define INC             29
#define INC_END         30
#define DEC_START       31
#define DEC             32
#define DEC_END         33
#define KADD_START      34
#define KASL            35
#define KADD_END        36
#define KINC_START      37
#define KIDL            38
#define KINC_END        39
#define KDEC_START      40
#define KDEC_END        41

#define LAST_ONE        42

BOOL scheduled;
int PARAM;
char *macro[LAST_ONE]; /* macro text */ 

char *functions[]={"MULTIPLY","MULTUP","SQUARE","REDC","ADDITION","INCREMENT",
                 "SUBTRACTION","DECREMENT","SUMMATION","INCREMENTATION",
                 "DECREMENTATION",NULL};

char *names[]={"MUL_START","STEP","STEP1M","STEP1A","STEP2M",
               "STEP2A","MFIN","MUL_END","LAST","SQR_START","DSTEP",
               "DSTEP1M","DSTEP1A","DSTEP2M","DSTEP2A","SELF",
               "SFIN","SQR_END","REDC_START","RFINU","RFIND",
               "REDC_END","ADD_START","ADD","ADD_END","SUB_START","SUB",
               "SUB_END","INC_START","INC","INC_END","DEC_START","DEC",
               "DEC_END","KADD_START","KASL","KADD_END","KINC_START","KIDL",
               "KINC_END","KDEC_START","KDEC_END",NULL};

BOOL white(char c)
{
    if (c==' ' || c=='\n' || c=='\r' || c=='\t') return TRUE;
    else return FALSE;
}

int skip(char *c,int i)
{
    while (white(c[i])) i++;
    return i;
}

int which(char *name,char *names[])
{
    int ipt=0;  
    while (names[ipt]!=NULL)
    {
        if (strcmp(name,names[ipt])==0) return ipt;
        ipt++;
    }
    return -1;
}

void m_prologue(FILE *dotc,int k,int m)
{
    fprintf(dotc,macro[STEP1M],k,m); 
}

void m_epilogue(FILE *dotc,int x)
{
    if (x==1) fprintf(dotc,macro[STEP1A]);
    else      fprintf(dotc,macro[STEP2A]);
}

void m_schedule(FILE *dotc,int x,int k,int m)
{
    if (x==1)
    {
        fprintf(dotc,macro[STEP2M],k,m);
        fprintf(dotc,macro[STEP1A]);
    }
    else
    {
        fprintf(dotc,macro[STEP1M],k,m);
        fprintf(dotc,macro[STEP2A]);
    }
}

void s_prologue(FILE *dotc,int k,int m)
{
    fprintf(dotc,macro[DSTEP1M],k,m);
}

void s_epilogue(FILE *dotc,int x)
{
    if (x==1) fprintf(dotc,macro[DSTEP1A]);
    else      fprintf(dotc,macro[DSTEP2A]);
}

void s_schedule(FILE *dotc,int x,int k,int m)
{
    if (x==1)
    {
        fprintf(dotc,macro[DSTEP2M],k,m);
        fprintf(dotc,macro[DSTEP1A]);
    }
    else
    {
        fprintf(dotc,macro[DSTEP1M],k,m);
        fprintf(dotc,macro[DSTEP2A]);
    }
}

/* Insert functions into template file */

void insert(int index,FILE *dotc)
{
    int i,j,k,m,n,x;
    switch (index)
    {
    case MULTIPLY:  
        fprintf(dotc,macro[MUL_START]);
        for (i=n=0;i<PARAM;i++,n++)
        {    
            k=0; m=i;

            if (scheduled)
            {
                x=1;
                m_prologue(dotc,k,m);
                k++; m--;

                while (k<=i)
                {
                    m_schedule(dotc,x,k,m);
                    k++; m--;
                    x=3-x;
                }

                m_epilogue(dotc,x);

            }
            else
            {
                while (k<=i)
                    fprintf(dotc,macro[STEP],k++,m--);
            }
            fprintf(dotc,macro[MFIN],n);
        }
        for (i=0;i<PARAM-1;i++,n++)
        {
            k=i+1; m=PARAM-1;

            if (scheduled)
            {
                x=1;
                m_prologue(dotc,k,m);
                k++; m--;

                while (k<=PARAM-1)
                {
                    m_schedule(dotc,x,k,m);
                    k++; m--;
                    x=3-x;
                }
                m_epilogue(dotc,x);
            }
            else
            {
                while (k<=PARAM-1)
                    fprintf(dotc,macro[STEP],k++,m--);
            }
            fprintf(dotc,macro[MFIN],n);
        }
        fprintf(dotc,macro[MUL_END],2*PARAM-1);
        break;
    case MULTUP:
        fprintf(dotc,macro[MUL_START]);
        for (i=n=0;i<PARAM-1;i++,n++)
        {    
            k=0; m=i;

            if (scheduled)
            {   
                x=1;
                m_prologue(dotc,k,m);
                k++; m--;

                while (k<=i)
                {
                    m_schedule(dotc,x,k,m);
                    k++; m--;
                    x=3-x;
                }
                m_epilogue(dotc,x);
            }
            else
            {
                while (k<=i)
                    fprintf(dotc,macro[STEP],k++,m--);
            }
            fprintf(dotc,macro[MFIN],n);
        }
        k=0; m=PARAM-1;
        while (k<=i)
            fprintf(dotc,macro[LAST],k++,m--);
        fprintf(dotc,macro[MUL_END],PARAM-1);
        break;
    case SQUARE:   
        fprintf(dotc,macro[SQR_START]);
        for (i=n=0;i<PARAM;i++,n++)
        {
            k=0; m=i;

            if (scheduled)
            {  
                if (k<m)
                {
                    x=1;
                    s_prologue(dotc,k,m);
                    k++; m--;

                    while (k<m)
                    {
                        s_schedule(dotc,x,k,m);
                        k++; m--;
                        x=3-x;
                    }
                   s_epilogue(dotc,x);
                }
            }
            else
            {
                while (k<m)
                    fprintf(dotc,macro[DSTEP],k++,m--);
            }
            if (n%2==0) fprintf(dotc,macro[SELF],n/2,n/2);

            fprintf(dotc,macro[SFIN],n);
        }
        for (i=0;i<PARAM-1;i++,n++)
        {
            k=i+1; m=PARAM-1;

            if (scheduled)
            {
                if (k<m)
                {
                    x=1;
                    s_prologue(dotc,k,m);
                    k++; m--;

                    while (k<m)
                    {
                        s_schedule(dotc,x,k,m);
                        k++; m--;
                        x=3-x;
                    }
                    s_epilogue(dotc,x);
                }
            }
            else
            {
                while (k<m)
                    fprintf(dotc,macro[DSTEP],k++,m--);
            }
            if (n%2==0) fprintf(dotc,macro[SELF],n/2,n/2);     
            fprintf(dotc,macro[SFIN],n);
        }
        fprintf(dotc,macro[SQR_END],2*PARAM-1);
        break;
    case REDC:  
        fprintf(dotc,macro[REDC_START]);
        fprintf(dotc,macro[RFINU],0,0);

        for (i=n=1;i<PARAM;i++,n++)
        {
            k=0; m=i;

            if (scheduled)
            {
                x=1;
                m_prologue(dotc,k,m);
                k++; m--;

                while (k<i)
                {
                    m_schedule(dotc,x,k,m);
                    k++; m--;
                    x=3-x;
                }
                m_epilogue(dotc,x);
            }
            else
            {
                while (k<i)
                    fprintf(dotc,macro[STEP],k++,m--);
            }
            fprintf(dotc,macro[RFINU],n,n);
        }
        for (i=0;i<PARAM-1;i++,n++)
        {
            k=i+1; m=PARAM-1;

            if (scheduled)
            {
                x=1;
                m_prologue(dotc,k,m);
                k++; m--;

                while (k<=PARAM-1)
                {
                    m_schedule(dotc,x,k,m);
                    k++; m--;
                    x=3-x;
                }
                m_epilogue(dotc,x);
            }
            else
            {
                while (k<=PARAM-1)
                    fprintf(dotc,macro[STEP],k++,m--);
            }
            fprintf(dotc,macro[RFIND],n,n);
        }
        fprintf(dotc,macro[REDC_END],2*PARAM-1,2*PARAM-1);
        break;
    case ADDITION:    
        fprintf(dotc,macro[ADD_START]);
        for (i=1;i<PARAM;i++)
            fprintf(dotc,macro[ADD],i,i,i);
        fprintf(dotc,macro[ADD_END]);
        break;
    case INCREMENT:
        fprintf(dotc,macro[INC_START]);
        for (i=1;i<PARAM;i++)
            fprintf(dotc,macro[INC],i,i,i);
        fprintf(dotc,macro[INC_END]);
        break;
    case SUBTRACTION:
        fprintf(dotc,macro[SUB_START]);
        for (i=1;i<PARAM;i++)
            fprintf(dotc,macro[SUB],i,i,i);
        fprintf(dotc,macro[SUB_END]);
        break;
    case DECREMENT:
        fprintf(dotc,macro[DEC_START]);
        for (i=1;i<PARAM;i++)
            fprintf(dotc,macro[DEC],i,i,i);
        fprintf(dotc,macro[DEC_END]);
        break;
    case SUMMATION:
        fprintf(dotc,macro[KADD_START],1);
        for (i=0;i<PARAM;i++)
            fprintf(dotc,macro[ADD],i,i,i);
        fprintf(dotc,macro[KASL],2,PARAM,PARAM,PARAM,1,2);
        fprintf(dotc,macro[KADD_END]);
        break;
    case INCREMENTATION:
        fprintf(dotc,macro[KINC_START],3);
        for (i=0;i<PARAM;i++)
            fprintf(dotc,macro[INC],i,i,i);
        fprintf(dotc,macro[KIDL],4,PARAM,PARAM,3,4);
        fprintf(dotc,macro[KINC_END]);
        break;
    case DECREMENTATION:
        fprintf(dotc,macro[KDEC_START],5);
        for (i=0;i<PARAM;i++)
            fprintf(dotc,macro[DEC],i,i,i);
        fprintf(dotc,macro[KIDL],6,PARAM,PARAM,5,6);
        fprintf(dotc,macro[KDEC_END]);
        break;
    default:
        break;
    }
}

int main(int argc,char **argv)
{
    FILE *template,*macros,*dotc;
    int i,ip,ptr,index,size;
    BOOL open,error;
    char fname[80],tmpl[80],name[20];
    char line[133];
    argc--; argv++;
    if (argc<3 || argc>4)
    {
       printf("Bad arguments\n");
       printf("mex <parameter> <.mcs file> <.tpl file>\n");
       printf("Use flag -s for scheduled code\n");
       printf("Examples:\n");
       printf("mex 6 ms86 mrcomba\n");
       printf("mex -s 8 c mrkcm\n");
       exit(0);
    }
    ip=0;
    scheduled=FALSE;
    if (strcmp(argv[0],"-s")==0)
    {
        ip=1;
        scheduled=TRUE;
    }

    PARAM=atoi(argv[ip]);
    if (PARAM<4 || PARAM>32)
    {
        printf("Invalid parameter\n");
        exit(0);
    }
    strcpy(fname,argv[ip+1]);
    strcat(fname,".mcs");
    macros=fopen(fname,"r");
    if (macros==NULL)
    {
        printf("Macro file %s not found\n",fname);
        exit(0);
    }

    strcpy(tmpl,argv[ip+2]);
    strcat(tmpl,".tpl");
    template=fopen(tmpl,"r");
    if (template==NULL)
    {
        printf("Template file %s file not found\n",tmpl);
        exit(0);
    }
    strcpy(tmpl,argv[ip+2]);
    strcat(tmpl,".c");
    dotc=fopen(tmpl,"w");
    if (dotc==NULL)
    {
        printf("Unable to open %s for output\n",tmpl);
        exit(0);
    }

    for (i=0;i<LAST_ONE;i++) macro[i]=NULL;

/* read in the macros - first pass to determine size and check for errors */
    open=error=FALSE;
    while (1)
    {
        if (fgets(line,132,macros)==NULL) break;
        if (line[0]==';') continue;
        
        if (!open && strncmp(line,"MACRO",5)==0) 
        {
                open=TRUE;
                ptr=6; i=0;
                ptr=skip(line,ptr);
                while (!white(line[ptr])) name[i++]=line[ptr++];
                name[i]='\0';
                index=which(name,names);
                if (index<0)
                {
                    error=TRUE;
                    break;
                }
                size=0;
                continue;
        }
        if (open && strncmp(line,"ENDM",4)==0) 
        {
                open=FALSE;
                macro[index]=(char *)malloc(size+1);
                macro[index][0]='\0';
        }

        if (open) size+=strlen(line);
    }
    fclose(macros);
    if (error)
    {
        printf("no such macro - %s\n",name);
        exit(0);
    }

/* read in the macros - second pass to store macros */     
    macros=fopen(fname,"r");   
    while (1)
    {
        if (fgets(line,132,macros)==NULL) break;
        if (line[0]==';') continue;

        if (!open && strncmp(line,"MACRO",5)==0) 
        {
                open=TRUE;
                ptr=6; i=0;
                ptr=skip(line,ptr);
                while (!white(line[ptr])) name[i++]=line[ptr++];
                name[i]='\0';
                index=which(name,names);
                continue;
        }
        if (open && strncmp(line,"ENDM",4)==0) open=FALSE;

        if (open) strcat(macro[index],line);
    }
    fclose(macros);

    if (scheduled && macro[STEP1M]==NULL)
    {
        printf("Error - scheduling not supported in file %s\n",fname);
        exit(0);
    }

/* Insert macros into dotc file */
    
    while (1)
    {
        if (fgets(line,132,template)==NULL) break;
        fputs(line,dotc);
        if (strncmp(line,"/***",4)==0)
        {
                ptr=4; i=0;
                ptr=skip(line,ptr);
                while (!white(line[ptr])) name[i++]=line[ptr++];
                name[i]='\0';

                index=which(name,functions);
             /*   printf("Recognize %s index %d\n",name,index);    */
                if (index<0)
                {
                    error=TRUE;
                    break;
                }
                insert(index,dotc);
        }
    }
    
    if (error)
        printf("no such function - %s\n",name);
   
    fclose(template);
    fclose(dotc);
    return 0;
}

