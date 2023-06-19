/*-----------------------------------------------------------------------------------*/
/* FILE			: FUNCTION.H      											         */
/* MODULE		: FUNCTION         											         */
/* AUTHOR		: HGU SASLAB 												         */
/* DATE			: 2016. 2.29.												         */
/*-----------------------------------------------------------------------------------*/

#ifndef HEADER_FUNCTION
#define HEADER_FUNCTION

/*-----------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <memory.h>
#include <time.h>
#include <windows.h>
#include <tchar.h>

/*-----------------------------------------------------------------------------------*/

#include "Common.h"

/*-----------------------------------------------------------------------------------*/

#pragma warning(disable: 4996) /* _CRT_SECURE_NO_WARNINGS */

/*-----------------------------------------------------------------------------------*/
/* Type Definitions of Variables											         */
/*-----------------------------------------------------------------------------------*/
/* INDEX : C_array index													         */
/* NSIZE : size of array													         */
/* COUNT : temporary count													         */
/* LOGIC : returens of logic check results of function (NOK, YOK, and so on)         */
/* MODE  : simulation mode (example: IDEAL_MODEL,SIMPLE_MODEL,PRECISE_MODEL)         */
/* FLAG  : flag (NOK, YOK)													         */
/* REAL  : float or doulbe constant or array								         */
/* DCM   : direction cosine matrix											         */
/* VEC2  : 2x1 vector														         */
/* VEC3  : 3x1 vector														         */
/* VEC4  : 4x1 vector														         */
/* QUAT  : quaternion														         */
/* MAT3  : 3x3 matrix														         */
/*-----------------------------------------------------------------------------------*/
typedef int                     CODE;
typedef long int				COUNT;
typedef unsigned int			INDEX, NSIZE;
typedef	unsigned short			LOGIC, MODE, FLAG, PORT;
typedef	double					REAL, DCM, VEC2, VEC3, VEC4, QUAT, MAT3, DOU;

typedef char* IP;
typedef unsigned char			CONF;
typedef	unsigned long			BAUD, ID;

/*-----------------------------------------------------------------------------------*/
/* File Handle Identifier													         */
/*-----------------------------------------------------------------------------------*/

#define NULL_DATA				(int  )(			-97531)
#define ID_END_OF_DATA			(int  )(			139812)
#define ID_END_DATA				(int  )(			139812)
#define ID_END_DATA_I			(int  )(			139812)
#define ID_END_DATA_UI			(int  )(			139812)
#define ID_END_DATA_US			(int  )(			139812)



/*-----------------------------------------------------------------------------------*/
/* Array Length																         */
/*-----------------------------------------------------------------------------------*/

#define MAX_STRING				(NSIZE)(			  1500)

#define MAX_LOADSTRING			(NSIZE)(			  1500)
#define BUF_LENGTH				(NSIZE)(			 60000)

/*-----------------------------------------------------------------------------------*/
/* Math Function Macro													             */
/*-----------------------------------------------------------------------------------*/

#define LOOP(Count,End)         for( Count=0; Count<End; Count++ )
#define LOOP2(Ct1,Ct2,Ed1,Ed2)	for( Ct1=0; Ct1<Ed1; Ct1++ ) for( Ct2=0; Ct2<Ed2; Ct2++ )
#define FIND(Count)             for( Count=0; ; Count++ )
#define SCAN_DATA()				ScanData(pFile)

#define GAUSS(m,s)				gauss(m,s)

#define GET_DIM(X)              (sizeof(X) / sizeof(X[0]))
#define MAG(X)					Magnitude( X, GET_DIM(X) )
#define SUM(X)					Summation( X, GET_DIM(X) )
#define ROUND(X)                ((X>=0.0) ? ((int)(X+0.5)) : ((int)(X-0.5)))

#define MAX(X,Y)                ((X > Y) ? X : Y) 
#define MIN(X,Y)                ((X < Y) ? X : Y) 
#define LIMIT(X,X_LIM)          (MAX( MIN(X,fabs(X_LIM)), -fabs(X_LIM) ))
#define	BOUND(X,X_LIM)			(MAX(fabs(X),X_LIM)*SIGN(X))
#define SIGN(X)                 ((X>=0.0)? 1.0 : -1.0 )
#define SIGN_VAL(Value,Sign)    ((Sign >= 0.0) ? Value : -Value)
#define TOGGLE(X)				((X==0)? 1 : 0)

#define SQUARE(X)               ((X)*(X))
#define CUBIC(X)                ((X)*(X)*(X))
#define QUAD(X)                 ((X)*(X)*(X)*(X))
#define PENTA(X)                ((X)*(X)*(X)*(X)*(X))

/*-----------------------------------------------------------------------------------*/
/* Macro Functions                                                                   */
/*-----------------------------------------------------------------------------------*/

#define MAT2_MULT(A,B,Ncol_D,D)     MatMult(A[0],B[0],D[0],GET_DIM(A),GET_DIM(B),Ncol_D)
#define MAT3_MULT(A,B,C,Ncol_D,D)   Mat3Mult(A[0],B[0],C[0],GET_DIM(A),GET_DIM(B),GET_DIM(C),Ncol_D,D[0])
#define MAT_TRANS(A,trA)            Trans( A[0], trA[0], GET_DIM(A), GET_DIM(trA) )
#define INV_MAT(matP,invP)          InvMat(matP[0],GET_DIM(matP),invP[0])

/*-----------------------------------------------------------------------------------*/
/* Initialize Function Macro												         */
/*-----------------------------------------------------------------------------------*/

#define SET_DCM( X )            SetDCM  ( X )
#define SET_MAT( X, Value )     SetMat33( X, Value )
#define SET_VEC( X, Value )     SetArray( X, Value, GET_DIM(X) )

#define INIT_BUF( X )           {memset(&X, 0, sizeof(X));}
#define INIT_DCM( cA_B )        {GetDCM(0.0,0.0,0.0,cA_B);}

/*-----------------------------------------------------------------------------------*/
/* Interface Function Macro												             */
/*-----------------------------------------------------------------------------------*/

#define INDAT(DATA,BUF)	        {DATA = BUF.DATA;}
#define INVEC(ARAY,BUF)         {memcpy( ARAY, BUF.ARAY, sizeof(ARAY));}

#define OUDAT(BUF,DATA)         {BUF.DATA = DATA;}
#define OUVEC(BUF,ARAY)         {memcpy( BUF.ARAY, ARAY, sizeof(ARAY));}

/*-----------------------------------------------------------------------------------*/
/* Data Print-out Function Macro                                                     */
/*-----------------------------------------------------------------------------------*/
#define      PRT(X,UNT )                       {fprintf( pFile, "%14.5f\t", X[i]*UNT     );}
#define TIME_PRT(X,UNT )                       {fprintf( tFILE, "%14.5f\t", X[i]*UNT     );}
#define FLG_PRT(X,UNT )                       {fprintf( pFile, "%14d\t", X[i]*(int)(UNT));}
#define VEC_PRT(X,UNT ) {LOOP(j,GET_DIM(X[i])) fprintf( pFile, "%14.5f\t", X[i][j]*UNT  );}
#define STR_PRT(X     )                       {fprintf( pFile, "%s",   X                );}

/*-----------------------------------------------------------------------------------*/
/* String Print-out Function Macro                                                   */
/*-----------------------------------------------------------------------------------*/

#define     MSG(   X,UNT)       printf("%10.3lf", X*UNT) 
#define STR_MSG(strX    )       printf("%s", strX) 
#define VEC_MSG(vecX,UNT)       {LOOP(j, GET_DIM(vecX)) MSG(vecX[j], UNT);} 

/*-----------------------------------------------------------------------------------*/
/* Data Memory Out Function Macro                                                    */
/*-----------------------------------------------------------------------------------*/

#define     WRT(BUF,DATA)                             {BUF[Time.count-1]    = DATA   ;}
#define VEC_WRT(BUF,ARAY)      {LOOP(i, GET_DIM(ARAY)) BUF[Time.count-1][i] = ARAY[i];}

/*-----------------------------------------------------------------------------------*/
/* Common                                                                            */
/*-----------------------------------------------------------------------------------*/

enum	LogicFlag { NOK, YOK };              // 0, 1
enum    LogicValue { LOW, HIGH };
enum    IdxVector { X, Y, Z, XY = 2, XYZ };  // 0, 1, 2, 2, 3
enum    IdxAngularRate { P, Q, R, PQR };
enum    IdxVelocity { U, V, W, UVW };
enum	IdxNEDFrame { N, E, D, NED };
enum	IdxControlPlane { DS, DB, DR, SBR };
enum    IdxAngle { PSI, THE, PHI, ANG2 = 2, ANG3 };
enum    IdxMisAlignment { ALP, BET, GAM };
enum    IdxGeodeticPos { LAT, LON, HGT, POS3 };

/*-----------------------------------------------------------------------------------*/
/* Math Functions                                                                    */
/*-----------------------------------------------------------------------------------*/

REAL    InnerProduct(REAL vecX[], REAL vecY[], NSIZE Dim);
void    CrossProduct(REAL vecX[], REAL vecY[], REAL vecZ[]);

void    SetArray(REAL vecX[], REAL value, NSIZE Dim);
void    SetMat33(REAL matA[][3], REAL value);
void    SetMat(REAL matA[], INDEX Nn, REAL value);
void	SetIDMat(REAL A[], INDEX Nn);
void    CopyMat(REAL matA[], REAL matB[], NSIZE Nm, NSIZE Nn);
void    SetDCM(REAL cA_B[][3]);

void    GetDCM(REAL psi, REAL the, REAL phi, REAL cA_B[][3]);
void    GetDCMtr(REAL psi, REAL the, REAL phi, REAL cB_A[][3]);

void    GetSkew(VEC3 vec[], REAL Skew[][3]);

void    Trans(REAL A[], REAL B[], NSIZE N, NSIZE M);
void    Trans33(REAL A[][3], REAL B[][3]);

REAL    DetMat22(REAL A[][2]);
REAL    DetMat33(REAL A[][3]);
REAL	DetMat44(REAL A[][4]);

void    InvMat22(REAL A[][2], REAL invA[][2]);
void    InvMat33(REAL A[][3], REAL invA[][3]);
void    InvMat44(REAL A[][4], REAL invA[][4]);
void    InvMat55(REAL A[][5], REAL invA[][5]);
void    InvMat66(REAL A[][6], REAL invA[][6]);
void	InvMat77(REAL A[][7], REAL invA[][7]);
void	InvMat88(REAL A[][8], REAL invA[][8]);

void	LUPFactorization(REAL InMat[], REAL L[], REAL U[], REAL Pv[], INDEX Nn);
void	InvL(REAL MatL[], REAL InvMatL[], INDEX Nn);
void	InvU(REAL MatU[], REAL InvMatU[], INDEX Nn);
void	MatMatSwapRow(REAL A[], REAL B[], INDEX Nn, INDEX ROW1, INDEX ROW2);


void	MatSum33(REAL A[][3], REAL B[][3], REAL C[][3]);
void    MatMult(REAL A[], REAL B[], REAL C[], NSIZE L, NSIZE M, NSIZE N);
void    MatMult21(REAL matA[][2], REAL vecX[], REAL vecY[]);
void    MatMult22(REAL matA[][2], REAL matB[][2], REAL matC[][2]);
void    MatMult31(REAL matA[][3], REAL vecX[], REAL vecY[]);
void    MatMult33(REAL matA[][3], REAL matB[][3], REAL matC[][3]);
void    Mat3Mult(REAL A[], REAL B[], REAL C[], NSIZE Nk, NSIZE Nl, NSIZE Nm, NSIZE Nn, REAL D[]);

void    VecSum21(REAL vecX[], REAL vecY[], REAL vecZ[]);
void    VecSum31(REAL vecX[], REAL vecY[], REAL vecZ[]);
void    VecSub21(REAL vecX[], REAL vecY[], REAL vecZ[]);
void    VecSub31(REAL vecX[], REAL vecY[], REAL vecZ[]);

void	Chol(REAL matIn[], REAL matOut[], NSIZE N);

void    DiscreteFirstOrderSystem(REAL vecX[], REAL vecXb[],
	const REAL Ca, const REAL Cb[], const NSIZE Dim,
	REAL vecRef[], REAL vecY[]);

void	GetFadingMemoryFilter3Gain(REAL bet, REAL Ts, VEC3 Gain[]);

/*-----------------------------------------------------------------------------------*/
/* data loading from text file                                                       */
/*-----------------------------------------------------------------------------------*/

FILE* FileOpen(char FileName[], char FileMode[]);
REAL    ScanData(FILE* pFile);
REAL    ScanData2(FILE* pFile);
void    ReadChar(FILE* pFile, char* buf);
void    LineFeed(FILE* pFile, NSIZE count);
void    Message(char* strMessage);
char* FileToken(char* FileName);

/*-----------------------------------------------------------------------------------*/
/* integration                                                                       */
/*-----------------------------------------------------------------------------------*/

void    DiscreteIntegral(REAL Ui[], REAL Ub[], REAL Yb[], REAL Ts, NSIZE Dim, REAL Out[]);

/*-----------------------------------------------------------------------------------*/
/* Get Window Time (returns [msec])                                                  */
/*-----------------------------------------------------------------------------------*/

double	GetWindowTime(void);

/*-----------------------------------------------------------------------------------*/
/* Shared Memory                                                                     */
/*-----------------------------------------------------------------------------------*/

//void SharedMem_CREATE(void);


/*-----------------------------------------------------------------------------------*/

#endif