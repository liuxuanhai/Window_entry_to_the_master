/*
 * Solve set of linear equations involving
 * a Hilbert matrix
 * i.e. solves   Hx=b, where b is the vector [1,1,1....1]
 *
 * Requires: flash.cpp
 *
 *      Copyright (c) Shamus Software 1988-1997
 */

#include <iostream>
#include <flash.h>

using namespace std;

Miracl precision=20;

static Flash A[20][20];
static Flash b[20];

BOOL gauss(Flash A[][20],Flash b[],int n)
{ /* solve Ax=b using Gaussian elimination *
   * solution returned in b                */ 
    int i,j,k,m;
    BOOL ok;
    Flash s;
    ok=TRUE;
    for (i=0;i<n;i++) A[i][n]=b[i];
    for (i=0;i<n;i++)
    { /* Gaussian elimination */
        m=i;
        for (j=i+1;j<n;j++) if (fabs(A[j][i])>fabs(A[m][i])) m=j;
        if (m!=i) for (k=i;k<=n;k++)
        {
            s=A[i][k];
            A[i][k]=A[m][k];
            A[m][k]=s;
        }
        if (A[i][i]==0)
        {
            ok=FALSE;
            break;
        }
        for (j=i+1;j<n;j++)
        {
            s=A[j][i]/A[i][i];
            for (k=n;k>=i;k--)  A[j][k]-=s*A[i][k];   
        }  
    }
    if (ok) for (j=n-1;j>=0;j--)
    { /* Backward substitution */
        s=0;
        for (k=j+1;k<n;k++)  s+=b[k]*A[j][k];
        if (A[j][j]==0)
        {
            ok=FALSE;
            break;
        } 
        b[j]=(A[j][n]-s)/A[j][j];
    }
    return ok;
}

int main()
{ /* solve set of linear equations */
    int i,j,n;
    miracl *mip=&precision;
    mip->RPOINT=OFF;   /* use fractions for output */
    do
    {
        cout << "Order of Hilbert matrix H= ";
        cin >> n;
    } while (n<2 || n>19);
    for (i=0;i<n;i++)
    {
        b[i]=1;
        for (j=0;j<n;j++)
              A[i][j]=(Flash)1/(i+j+1);  
    }
    if (gauss(A,b,n))
    {
        cout << "\nSolution is\n";
        for (i=0;i<n;i++)  cout << "x[" << i+1 << "] = " << b[i] << "\n";
        if (mip->EXACT) cout << "Result is exact!\n";
    }
    else cout <<  "H is singular!\n";
    return 0;
}

